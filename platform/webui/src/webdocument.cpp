#include "webdocument.h"
#include <QMessageBox>
#include <QSettings>
#include "webwidget.h"
#include "attestor.h"

#include <QFileDialog>
static int luatrans = LUA_REFNIL;
static int luatimer = LUA_REFNIL;
static int luaclose = LUA_REFNIL;
static lua_State * theState = nullptr;
static CWebWidget* theWidget= nullptr;

//设置LUA函数及启动业务逻辑处理
extern "C" int install(lua_State* L){
    theState = L;
    //unsigned int tmval  = (unsigned int)lua_tointeger(L,1);
    luaclose = luaL_ref(L,LUA_REGISTRYINDEX); //注意，这里是先进后出,第注册一次返回会+1
    luatimer = luaL_ref(L,LUA_REGISTRYINDEX);
    luatrans = luaL_ref(L,LUA_REGISTRYINDEX);
    return 0;
}

extern "C" int print(lua_State* L){
    theState = L;
    const char* s = lua_tostring(L,1);
    if( theWidget && s ){
        theWidget->Trace( s );
    }
    return 0;
}

//u:输入的lua脚本参数
extern "C" int option(lua_State* L){
    lua_pushstring(L, "myoption");
    return 1;
}

/**********************************************************************************************************/

//业务逻辑处理启动
bool CLua::start(const QString file, QWidget* parent){
    theWidget = (CWebWidget*)parent;
    theState = luaL_newstate();
    luaL_openlibs( theState );
    if(luaL_loadfile(theState, file.toUtf8().constData())){  //这里一定要loadfile，dofile致lua c++接口无效
        printf("script error or file:%s mission!\n", file.toUtf8().constData());
        lua_close(theState);
        theState = nullptr;
        return true;
    }
    //注册LUA c++接口
    lua_register( theState, "install",install);
    lua_register( theState, "print", print );
    lua_register( theState, "option", option );
    lua_pcall(theState, 0, LUA_MULTRET, 0);
    m_States[file] = theState;
    return true;
}

void CLua::request(const QString file, const QString func, const QString args){
    if( theWidget ){
        theWidget->Trace( QString("%1,%2,%3").arg(file).arg(func).arg(args));
    }
    QString rsp;
    QStringList arglist = args.split(",");
    QMap<QString, lua_State*>::iterator ifnd = m_States.find( file );
    if( ifnd != m_States.end()){
        //luaL_glock(ifnd.value());
        lua_getglobal(ifnd.value(), func.toUtf8().constData());
        for( int i = 0; i < arglist.size(); i++ ){
            lua_pushstring(ifnd.value(), arglist.at(i).toUtf8().constData() );
        }
        lua_pcall(ifnd.value(),arglist.size(),1,0);
        char* ret = (char*)lua_tostring(ifnd.value(),-1);
        //luaL_unglock(ifnd.value());
        if( ret ) rsp =ret;
    }else{
        lua_State * lua = luaL_newstate();
        luaL_openlibs( lua );
        if( luaL_dofile(lua, file.toUtf8().constData())){
            printf("%s,script error or file mission!\n",file.toUtf8().constData());
            lua_close(lua);
            lua = nullptr;
            rsp = "err script";
        }else{
            lua_getglobal(lua, func.toUtf8().constData());
            for( int i=0; i<arglist.size(); i++)
                lua_pushstring(lua, arglist.at(i).toUtf8().constData());
            lua_pcall(lua,arglist.size(),1,0);
            char* ret = (char*)lua_tostring(lua,-1);
            if( ret ) rsp =ret;
            m_States[file] = lua;
        }
    }
    emit response( rsp );
}

void CLua::request(const QString func, const QString sjson){
    if( theWidget ){
        theWidget->Trace( QString("%1:%2").arg(func).arg(sjson));
    }
    QString rsp;
    QStringList funcs = func.split(".");
    QMap<QString, lua_State*>::iterator ifnd = m_States.find( funcs[0] );
    if( ifnd != m_States.end()){
        //luaL_glock(ifnd.value());
        lua_getglobal(ifnd.value(), funcs[1].toUtf8().constData());
        lua_pushstring(ifnd.value(), sjson.toUtf8().constData() );
        lua_pcall(ifnd.value(),1,1,0);
        char* ret = (char*)lua_tostring(ifnd.value(),-1);
        //luaL_unglock(ifnd.value());
        if( ret ) rsp =ret;
    }else{
        lua_State * lua = luaL_newstate();
        luaL_openlibs( lua );
        if( luaL_dofile(lua, funcs[0].toUtf8().constData())){
            printf("%s,script error or file mission!\n",funcs[0].toUtf8().constData());
            lua_close(lua);
            lua = nullptr;
            rsp = "err script";
        }else{
            lua_getglobal(lua, funcs[1].toUtf8().constData());
            lua_pushstring(ifnd.value(), sjson.toUtf8().constData() );
            lua_pcall(ifnd.value(),1,1,0);
            char* ret = (char*)lua_tostring(lua,-1);
            if( ret ) rsp =ret;
           // m_States[file] = lua;
        }
    }
    emit response( rsp );
}

 /************************************* session ************************************************/
void CSession::add(const QString name, const QString value ){
    m_Sessions[ name ] = value;
}

void CSession::remove(const QString name ){
    QMap<QString, QString>::iterator ifnd = m_Sessions.find(name);
    if( ifnd != m_Sessions.end()){
         m_Sessions.erase(ifnd);
    }
}

void CSession::get(){
    QString strRet;
    QString strVal;
    QMap<QString, QString>::iterator it;
    for( it = m_Sessions.begin(); it != m_Sessions.end();it++){
        strVal += QString("'%1':'%2',").arg(it.key()).arg(it.value());
    }
    if( !strVal.isEmpty() ){
       strRet =  QString("{%1}").arg(strVal.left( strVal.length()-1));
    }else{
       strRet = QString("{'empty'}");
    }
    emit list( strRet );
}

/****************************************cookies******************************************************/
void CCookies::add(const QString name, const QString value ){
    m_Cookies[ name ] = value;
    QString strRet;
    QString strVal;
    QMap<QString, QString>::iterator it;
    for( it = m_Cookies.begin(); it != m_Cookies.end();it++){
        strVal += QString("'%1':'%2',").arg(it.key()).arg(it.value());
    }
    if( !strVal.isEmpty() ){
       strRet =  QString("{%1}").arg(strVal.left( strVal.length()-1));
    }else{
       strRet = QString("{}");
    }
    QFile *file = new QFile;
    file->setFileName( "./cookies.dat" );
    if( file->open(QFile::WriteOnly|QFile::Truncate) ){
        QTextStream out(file);
        out<<strRet;
        file->close();
        delete(file);
    }
}

void CCookies::remove(const QString name ){
    QMap<QString, QString>::iterator ifnd = m_Cookies.find(name);
    if( ifnd != m_Cookies.end()){
         m_Cookies.erase(ifnd);
    }
}

void CCookies::get(){
    QString text = QString("{}");
    QFile *file = new QFile;
    file->setFileName( "./cookies.dat" );
    if( file->open(QIODevice::ReadOnly) ){
         QTextStream in(file);
         text = in.readAll();
         file->close();
         delete(file);
     }
     emit list( text );
}

/****************************************file******************************************************/
void CFile::save(const QString name, const QString text ){
    QFile *file = new QFile;
    file->setFileName( name );
    if( file->open(QIODevice::WriteOnly) ){
        QTextStream out(file);
        out<<text;
        file->close();
        delete(file);
    }
}

void CFile::load(const QString name ){
    QString text = QString("{}");
    QFile *file = new QFile;
    file->setFileName( name );
    if( file->open(QIODevice::ReadOnly) ){
         QTextStream in(file);
         text = in.readAll();
         file->close();
         delete(file);
     }
    emit context( text );
}

void CFile::saveto(const QString name, const QString filter, const QString text ){
    QString filepath = QFileDialog::getSaveFileName(nullptr,"请选择导出文件保存路径",QString("%1/%2").arg(QDir::homePath()).arg(name),filter);
    if(filepath.isEmpty())
        return;
    QFile *file = new QFile;
    file->setFileName( filepath );
    if( file->open(QIODevice::WriteOnly) ){
        QTextStream out(file);
        out<<text;
        file->close();
        delete(file);
    }
}

/****************************************CWarrant******************************************************/
void CWarrant::get(){
    QSettings * cfg = new QSettings("config.ini", QSettings::IniFormat);
    if( cfg ){
        cfg->setIniCodec("utf-8");
        cfg->beginGroup("app");
        QString prikey =  cfg->value("key","unautorized").toString();
        attestor::identify idf;
        attestor::prove((char*)(prikey.toStdString().c_str()),idf);
        emit info( QString("{'key':'%1','expire':'%2'}").arg(prikey).arg(idf.expire));
    }
}

/****************************************Command******************************************************/
void CCommand::show( bool  isVisiable ){
    if( isVisiable ){
        theWidget->show();
    }else{
        theWidget->hide();
    }
}

void CCommand::move( int x, int y ){
    theWidget->move(x, y );
}
