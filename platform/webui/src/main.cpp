#include <QApplication>
#include <QtWebEngine/QtWebEngine>
#include <QSettings>
#include "webwidget.h"
#include "attestor.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    QString dir = QApplication::applicationDirPath();
    a.setWindowIcon(QIcon("./smartx.ico"));

    //config
    QSettings * cfg = new QSettings("config.ini", QSettings::IniFormat);
    cfg->setIniCodec("utf-8");
    cfg->beginGroup("app");
    QString stitle   = cfg->value("title","smartx").toString();
    QString prikey   = cfg->value("key","").toString();
    QString homepag  = cfg->value("home","").toString();
    QString mainlua  = cfg->value("main","").toString();
    unsigned int fullscreen = cfg->value("fullscreen",0).toUInt();
    int  w  = cfg->value("width",800).toInt();
    int  h  = cfg->value("height",600).toInt();

    //show mainframe
    attestor::identify idf;
    memset(&idf,0x00,sizeof(idf));
    QString defaulturl("qrc:/error.html");
    if( !homepag.isEmpty() && attestor::prove((char*)(prikey.toStdString().c_str()),idf)){
        if( homepag.startsWith("http:/",Qt::CaseInsensitive))
            defaulturl = homepag;
        else
            defaulturl = QString("file:///%1/%2").arg(dir).arg(homepag);
    }
    CWebWidget webwidget;
    webwidget.setWindowTitle(QString("%1 - %2").arg(stitle).arg((char*)idf.mac));
    webwidget.load(mainlua,defaulturl);
    if( fullscreen > 0 )
       // webwidget.setWindowFlags(Qt::FramelessWindowHint);
        webwidget.showFullScreen();
    else{
        webwidget.setMinimumSize(w,h);
        //webwidget.show();
    }
    a.exec();
    return 0;
}
