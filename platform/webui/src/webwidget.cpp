#include "webwidget.h"
#include <QWebChannel>
#include <QWebEngineView>
#include <QMessageBox>
#include <QListWidget>
#include <QProcess>

CWebWidget::CWebWidget(QWidget *parent) :QWidget(parent),mainTrace(nullptr){
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainView = new QWebEngineView(this);
    mainPage = new QWebEnginePage(this);
    mainView->setParent(this);
    mainView->setPage( mainPage );
    mainView->setContextMenuPolicy(Qt::NoContextMenu);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("lua"), &m_luaobj);
    channel->registerObject(QStringLiteral("session"), &m_sessionobj);
    channel->registerObject(QStringLiteral("cookies"), &m_cookiesobj);
    channel->registerObject(QStringLiteral("file"), &m_fileobj);
    channel->registerObject(QStringLiteral("warrant"), &m_warrantobj);
    channel->registerObject(QStringLiteral("command"), &m_commandobj);
    mainPage->setWebChannel(channel);
    mainLayout->addWidget( mainView );
    mainTrace = new QListWidget(this);
    mainTrace->hide();
    mainLayout->addWidget( mainTrace );
    mainTrace->addItem("start trace");
    m_iQProcess = new QProcess( this );
    connect(m_iQProcess, SIGNAL(readyReadStandardOutput()) , this , SLOT(ReadOutput()));
    connect(m_iQProcess, SIGNAL(readyReadStandardError()) , this , SLOT(ReadOutput()));
    connect(m_iQProcess, SIGNAL(readyRead()) , this , SLOT(ReadOutput()));
    m_iQProcess->start();
    m_iQProcess->waitForStarted();
}

void CWebWidget::ReadOutput(){
    //QByteArray ba = m_iQProcess->readAllStandardOutput();
    //qDebug() << ba;
    //QMessageBox::information(this, "", "ba");
}

CWebWidget::~CWebWidget(){
}

void CWebWidget::load(const QString script, const QString url ){
   m_luaobj.start( script, this);
   mainPage->load(QUrl(url));
}

void CWebWidget::Trace(const QString text){
   if( mainTrace ){
       if( mainTrace->count() > 1000)
           mainTrace->clear();
        mainTrace->insertItem(-1,text);
    }
}

void CWebWidget::keyPressEvent(QKeyEvent *e){
    switch( e->key() ) {
    case Qt::Key_F5:
        mainView->reload();
        break;
    case Qt::Key_F12:
        if( mainTrace->isVisible() )
            mainTrace->hide();
        else
            mainTrace->show();
        break;
    case Qt::Key_Delete:
        mainTrace->clear();
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(e);
}

void CWebWidget::keyReleaseEvent(QKeyEvent *e){
    QWidget::keyReleaseEvent(e);
}

