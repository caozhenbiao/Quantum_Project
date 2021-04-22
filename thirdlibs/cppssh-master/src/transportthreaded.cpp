/*
    cppssh - C++ ssh library
    Copyright (C) 2015  Chris Desjardins
    http://blog.chrisd.info cjd@chrisd.info

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "transportthreaded.h"
#include "crypto.h"
#include "channel.h"
#include "debug.h"

CppsshTransportThreaded::CppsshTransportThreaded(const std::shared_ptr<CppsshSession>& session)
    : CppsshTransport(session)
{
}

CppsshTransportThreaded::~CppsshTransportThreaded()
{
    cdLog(LogLevel::Debug) << "~CppsshTransportThreaded";
    stopThreads();
}

void CppsshTransportThreaded::stopThreads()
{
    _running = false;
    if (_rxThread.joinable() == true)
    {
        _rxThread.join();
    }
    if (_txThread.joinable() == true)
    {
        _txThread.join();
    }
}

bool CppsshTransportThreaded::startThreads()
{
    _rxThread = std::thread(&CppsshTransportThreaded::rxThread, this);
    _txThread = std::thread(&CppsshTransportThreaded::txThread, this);
    return true;
}

bool CppsshTransportThreaded::setupMessage(const Botan::secure_vector<Botan::byte>& buffer,
                                           Botan::secure_vector<Botan::byte>* outBuf)
{
    bool ret = true;
    size_t length = buffer.size();
    CppsshPacket out(outBuf);
    Botan::byte padLen;
    uint32_t packetLen;

    uint32_t encryptBlockSize = _session->_crypto->getEncryptBlockSize();
    if (encryptBlockSize == 0)
    {
        encryptBlockSize = 8;
    }

    padLen = (Botan::byte)(3 + encryptBlockSize - ((length + 8) % encryptBlockSize));
    packetLen = 1 + length + padLen;

    out.addInt(packetLen);
    out.addByte(padLen);
    out.addVector(buffer);

    Botan::secure_vector<Botan::byte> padBytes;
    padBytes.resize(padLen, 0);
    out.addVector(padBytes);
    return ret;
}

bool CppsshTransportThreaded::sendMessage(const Botan::secure_vector<Botan::byte>& buffer)
{
    bool ret;
    Botan::secure_vector<Botan::byte> buf;
    setupMessage(buffer, &buf);
    ret = CppsshTransport::sendMessage(buf);
    return ret;
}

void CppsshTransportThreaded::rxThread()
{
    cdLog(LogLevel::Debug) << "starting rx thread";
    try
    {
        Botan::secure_vector<Botan::byte> incoming;
        size_t size = 0;
        while (_running == true)
        {
            if (incoming.size() < sizeof(uint32_t))
            {
                size = sizeof(uint32_t);
            }
            if (receiveMessage(&incoming, size) == true)
            {
                CppsshPacket packet(&incoming);
                size = packet.getCryptoLength();
                if (incoming.size() >= size)
                {
                    processIncomingData(&incoming, incoming, size);
                    size = packet.getCryptoLength();
                }
            }
            else
            {
                break;
            }
        }
    }
    catch (const std::exception& ex)
    {
        cdLog(LogLevel::Error) << "rxThread exception: " << ex.what();
        CppsshDebug::dumpStack(_session->getConnectionId());
    }
    _running = false;
    cdLog(LogLevel::Debug) << "rx thread done";
}

void CppsshTransportThreaded::txThread()
{
    cdLog(LogLevel::Debug) << "starting tx thread";
    try
    {
        while (_running == true)
        {
            if (_session->_channel->flushOutgoingChannelData() == false)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            sendKeepAlive();
        }
    }
    catch (const std::exception& ex)
    {
        cdLog(LogLevel::Error) << "txThread exception: " << ex.what();
        CppsshDebug::dumpStack(_session->getConnectionId());
    }
    cdLog(LogLevel::Debug) << "tx thread done";
}

bool CppsshTransportThreaded::processIncomingData(Botan::secure_vector<Botan::byte>* inBuf,
                                                  const Botan::secure_vector<Botan::byte>& incoming,
                                                  uint32_t dataLen) const
{
    bool dataProcessed = false;
    if ((_running == true) && (incoming.empty() == false))
    {
        dataProcessed = true;
        _session->_channel->handleReceived(incoming);
        if (inBuf->size() == dataLen)
        {
            inBuf->clear();
        }
        else
        {
            inBuf->erase(inBuf->begin(), inBuf->begin() + dataLen);
        }
    }
    return dataProcessed;
}
