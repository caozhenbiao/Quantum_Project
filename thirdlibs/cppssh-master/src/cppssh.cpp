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

#include "cppssh.h"
#include "impl.h"

std::shared_ptr<CppsshImpl> Cppssh::s_cppsshInst;
std::mutex Cppssh::s_cppsshInstMutex;

void Cppssh::create(int apiLevel)
{
    if (s_cppsshInst == nullptr)
    {
        std::unique_lock<std::mutex> lock(s_cppsshInstMutex);
        if (s_cppsshInst == nullptr)
        {
            // A quick check to make sure that the header files in an end program are the
            // same API level as the library was built with.
            if (apiLevel != getApiLevel())
            {
                cdLog(LogLevel::Error) <<
                    "API level defined in cppssh.h differs from API level in the cppssh library." << std::endl;
                cdLog(LogLevel::Error) << "Current API level: " << apiLevel << " API level in cppssh library: " <<
                    CPPSSH_API_LEVEL_CURRENT  << std::endl;
                abort();
            }
            s_cppsshInst.reset(new CppsshImpl());
        }
    }
}

void Cppssh::destroy()
{
    s_cppsshInst.reset();
}

const char* Cppssh::getCppsshVersion(bool detailed)
{
    const char* ret = CPPSSH_SHORT_VERSION;
    if (detailed == true)
    {
        ret = CPPSSH_FULL_VERSION;
    }
    return ret;
}

int Cppssh::getApiLevel()
{
    return CPPSSH_API_LEVEL_CURRENT;
}

CppsshConnectStatus_t Cppssh::connect(int* connectionId, const char* host, const short port, const char* username,
                                      const char* privKeyFile, const char* password, unsigned int timeout,
                                      const bool x11Forwarded, const bool keepAlives, const char* term)
{
    CppsshConnectStatus_t ret = CPPSSH_CONNECT_ERROR;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->connect(connectionId, host, port, username, privKeyFile, password, timeout, x11Forwarded,
                                  keepAlives, term);
    }
    return ret;
}

bool Cppssh::isConnected(const int connectionId)
{
    bool ret = false;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->isConnected(connectionId);
    }
    return ret;
}

bool Cppssh::writeString(const int connectionId, const char* data)
{
    return write(connectionId, (const uint8_t*)data, strlen(data));
}

bool Cppssh::write(const int connectionId, const uint8_t* data, size_t bytes)
{
    bool ret = false;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->write(connectionId, data, bytes);
    }
    return ret;
}

bool Cppssh::read(const int connectionId, CppsshMessage* data)
{
    bool ret = false;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->read(connectionId, data);
    }
    return ret;
}

bool Cppssh::windowChange(const int connectionId, const uint32_t cols, const uint32_t rows)
{
    bool ret = false;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->windowChange(connectionId, cols, rows);
    }
    return ret;
}

bool Cppssh::close(const int connectionId)
{
    bool ret = false;
    std::shared_ptr<CppsshImpl> cppsshInst = s_cppsshInst;
    if (cppsshInst != nullptr)
    {
        ret = cppsshInst->close(connectionId);
    }
    return ret;
}

bool Cppssh::setPreferredCipher(const char* prefCipher)
{
    return CppsshImpl::setPreferredCipher(prefCipher);
}

bool Cppssh::setPreferredHmac(const char* prefHmac)
{
    return CppsshImpl::setPreferredHmac(prefHmac);
}

size_t Cppssh::getSupportedCiphers(char* ciphers)
{
    return CppsshImpl::getSupportedCiphers(ciphers);
}

size_t Cppssh::getSupportedHmacs(char* hmacs)
{
    return CppsshImpl::getSupportedHmacs(hmacs);
}

bool Cppssh::generateRsaKeyPair(const char* fqdn, const char* privKeyFileName, const char* pubKeyFileName,
                                short keySize)
{
    return CppsshImpl::generateRsaKeyPair(fqdn, privKeyFileName, pubKeyFileName, keySize);
}

bool Cppssh::generateDsaKeyPair(const char* fqdn, const char* privKeyFileName, const char* pubKeyFileName,
                                short keySize)
{
    return CppsshImpl::generateDsaKeyPair(fqdn, privKeyFileName, pubKeyFileName, keySize);
}

CppsshMessage::CppsshMessage()
    : _message(nullptr),
    _len(0)
{
}

CppsshMessage::~CppsshMessage()
{
    if (_message != nullptr)
    {
        delete[] _message;
    }
}

const uint8_t* CppsshMessage::message() const
{
    return _message;
}

CppsshMessage& CppsshMessage::operator=(const CppsshMessage& other)
{
    setMessage(other._message, other._len);
    return *this;
}

void CppsshMessage::setMessage(const uint8_t* message, size_t bytes)
{
    _message = new uint8_t[bytes + 1];
    _len = bytes;
    memcpy(_message, message, _len);
    _message[_len] = 0;
}

size_t CppsshMessage::length() const
{
    return _len;
}
