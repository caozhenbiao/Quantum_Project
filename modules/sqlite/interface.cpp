extern "C" {
#include "../../platform/quark/liblua/src/lua.h"
#include "../../platform/quark/liblua/src/lualib.h"
#include "../../platform/quark/liblua/src/lauxlib.h"
}

#include "mysqlite.h"

mysqlite  * theSqlite = NULL;
lua_State * theState = NULL;

//����ͨ��
static int open( lua_State *L ){
	const char * file = luaL_checkstring(L,1);
	const char * pwd  = luaL_checkstring(L,2);
	int db = theSqlite->open( file,pwd );
	lua_pushinteger(L, db );
	return 1;
}

//execute
static int execute(lua_State *L){
	int  db = (int)luaL_checkinteger(L, 1);
	const char * sql = luaL_checkstring(L, 2);
	int ret = theSqlite->execute(db,sql);;
	lua_pushinteger(L,ret);
	return 1;
}

//stop
static int prepare(lua_State *L) {
	int   db = (int)luaL_checkinteger(L, 1);
	const char * sql = luaL_checkstring(L, 2);
	int mt = theSqlite->prepare(db, sql);;
	lua_pushinteger(L, mt);
	return 1;
}

static int setup(lua_State *L) {
	int   mt = (int)luaL_checkinteger(L, 1);
	int ret = theSqlite->setup(mt);;
	lua_pushinteger(L, ret);
	return 1;
}

static int bind_blod(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	size_t len = (size_t)luaL_checkinteger(L, 3);
	const char * buf = luaL_checklstring(L, 4, &len);
	int ret = theSqlite->bind_blob(mt, col, (void*)buf, (int)len );
	lua_pushinteger(L, ret);
	return 1;
}

static int bind_int(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	int val = (int)luaL_checkinteger(L, 3);
	int ret = theSqlite->bind_int(mt, col, val);
	lua_pushinteger(L, ret);
	return 1;
}

static int bind_text(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	const char* txt = theSqlite->column_text(mt, col);
	int ret = theSqlite->bind_text(mt, col, txt);
	lua_pushinteger(L, ret);
	return 1;
}


static int column_text(lua_State *L) {
	int mt  = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	const char* txt = theSqlite->column_text(mt, col);;
	lua_pushstring(L, txt);
	return 1;
}

static int column_int(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	int val = theSqlite->column_int(mt, col);;
	lua_pushinteger(L, val);
	return 1;
}

static int column_blob(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	int len = 0;
	const void* blob = theSqlite->column_blob(mt, col, len);
	lua_pushlstring(L, (const char*)blob, len );
	return 1;
}

static int column_float(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int col = (int)luaL_checkinteger(L, 2);
	double val = theSqlite->column_double(mt, col);;
	lua_pushnumber(L, val);
	return 1;
}


static int finalize(lua_State *L) {
	int mt = (int)luaL_checkinteger(L, 1);
	int ret = theSqlite->finalize(mt);;
	lua_pushinteger(L, ret);
	return 1;
}

static int table_count(lua_State *L) {
	int   db = (int)luaL_checkinteger(L, 1);
	const char * tname = luaL_checkstring(L, 2);
	const char * tcond = luaL_checkstring(L, 3);
	int ret = theSqlite->table_count( db, tname, tcond);
	lua_pushinteger(L, ret);
	return 1;
}

static int close(lua_State *L) {
	int db = (int)luaL_checkinteger(L, 1);
	int ret = theSqlite->close(db);;
	lua_pushinteger(L, ret);
	return 1;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static int destory(lua_State *L){
	if (theSqlite) {
		delete theSqlite;
		theSqlite = NULL;
	}
	return 0;
}

//
static const struct luaL_Reg mylib[]={
	{"destory",destory},
	{"open",open },
	{"execute",execute },
	{"prepare",prepare },
	{"setup",setup },
	{ "bind_blod",bind_blod },
	{ "bind_int",bind_int },
	{ "bind_text",bind_text },
	{ "column_text",column_text },
	{ "column_int",column_int },
	{ "column_float",column_float },
	{ "column_blob",column_blob },
	{ "finalize",finalize },
	{"table_count",table_count},
	{ "close",close },
	{NULL,NULL}
};

#ifdef WIN32
	extern "C" __declspec(dllexport) int luaopen_sqlite(lua_State *L){
#else
	extern "C" int luaopen_sqlite(lua_State *L){
#endif
	printf("luaopen_sqlite\n");
	if( !theState ){
		theState = L;
		theSqlite = new mysqlite;
	}
	lua_newtable(L);
	const luaL_Reg * la = &mylib[0];
	for( ; la->name!=NULL; la++ ){
		lua_pushcclosure(L,la->func, 0);
		lua_setfield(L,-2,la->name);
	}
	lua_pop(L,0);
	return 1;
}