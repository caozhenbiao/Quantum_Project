#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QWebEngineView>
#include "webdocument.h"
#include <QListWidget>
#include <QProcess>
#include <QKeyEvent>

class CWebWidget : public QWidget{
    Q_OBJECT
public:
    explicit CWebWidget(QWidget *parent = 0);
    ~CWebWidget();
public:
    void load(const QString script, const QString url);
    void Trace(const QString text);
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void keyReleaseEvent(QKeyEvent * e);

private slots:
    void ReadOutput();

private:
    QWebEngineView * mainView;
    CLua             m_luaobj;
    CSession         m_sessionobj;
    CCookies         m_cookiesobj;
    CFile            m_fileobj;
    CWarrant         m_warrantobj;
    QVBoxLayout *    mainLayout;
    QWebEnginePage * mainPage;
    QListWidget *    mainTrace;
    QProcess *       m_iQProcess;
    CCommand      m_commandobj;
};

#endif // MAINWIDGET_H
