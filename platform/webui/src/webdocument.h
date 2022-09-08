#ifndef WEBDOCUMENT_H
#define WEBDOCUMENT_H

#include <QObject>
#include <QMap>

extern "C" {
#include "../quark/liblua/src/lua.h"
#include "../quark/liblua/src/lauxlib.h"
#include "../quark/liblua/src/lualib.h"
}

class CLua : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY response)
public:
    explicit CLua(QObject *parent = nullptr):QObject(parent){}
public:
    bool start(const QString file, QWidget* parent);
public slots:
    void request(const QString file, const QString func, const QString args);
    void request(const QString func, const QString sjson);
signals:
    void response(const QString &text);
private:
    QString s_text;
    QMap<QString, lua_State*> m_States;
};

//sessioin
class CSession : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY list)
public:
    explicit CSession(QObject *parent = nullptr):QObject(parent){}
public slots:
    void add(const QString name, const QString value);
    void remove(const QString name);
    void get();
signals:
    void list(const QString &text);
private:
    QString s_text;
    QMap<QString, QString> m_Sessions;
};

//cookies
class CCookies : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY list)
public:
    explicit CCookies(QObject *parent = nullptr):QObject(parent){}
public slots:
    void add(const QString name, const QString value);
    void remove(const QString name);
    void get();
signals:
    void list(const QString &text);
private:
    QString s_text;
    QMap<QString, QString> m_Cookies;
};

//file
class CFile : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY context)
public:
    explicit CFile(QObject *parent = nullptr):QObject(parent){}
public slots:
    void save(const QString name, const QString text);
    void saveto(const QString name, const QString filter, const QString text);
    void load(const QString name);
signals:
    void context(const QString &text);
private:
    QString s_text;
};


//CWarrant
class CWarrant : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY info)
public:
    explicit CWarrant(QObject *parent = nullptr) : QObject(parent){}
public slots:
    void get();
signals:
    void info(const QString &text);
private:
    QString s_text;
};

//CCommand
class CCommand : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER s_text NOTIFY context)
public:
    explicit CCommand(QObject *parent = nullptr):QObject(parent){}
public slots:
    void show( bool isVisiable);
    void move( int x, int y );
signals:
    void context(const QString &text);
private:
    QString s_text;
};


#endif // WEBDOCUMENT_H
