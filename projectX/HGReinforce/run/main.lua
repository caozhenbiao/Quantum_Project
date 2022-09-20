--*****LUA SERMON SCRIPT*****--
print("start HG project at:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local JSON   = require "luajson"
local CSV    = require "luacsv"
local XAPI   = require "luaxapi"
local MD5    = require "luamd5"
local BITX   = require "luabit"

--local OPENCV = require "opencv"
local SYS    = require "system"
local xSQL   = require "sqlite"
local TaskA  = require("main_alc")
require("main_sec")
require("main_ips")
local currentModel = {};
local currentTable = {};
local currentUser = "KEVIN";
local scanPos = 0;
local scanStatus = 0;
local mydb = 0;

------------------------------------------------------管理操作-------------------------------------------------------------------------------
--初始数据库存
function factorysetup()
	if os.getenv("OS") == "Windows_NT" then
		os.execute("rmdir /s/q logs");
		os.execute("rmdir /s/q data");
	else
		os.execute("rm -r -f ./logs");
		os.execute("rm -r -f ./data");
	end
	local sqls={};
	sqls[1] = "DROP TABLE operatrec";
	sqls[2] = "DROP TABLE users";
	sqls[3] = "DROP TABLE reinforce";
	sqls[4] = "CREATE TABLE operatrec(autoid INTEGER PRIMARY KEY AUTOINCREMENT, user VARCHAR(20) NOT NULL, code VARCHAR(4) NOT NULL,type INT(4) NOT NULL,result INT(4) NOT NULL, context TEXT(500), optime TIMESTAMP DEFAULT (datetime('now','localtime')))";
	sqls[5] = "CREATE TABLE users(autoid INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,account VARCHAR(30) NOT NULL,pwdmd5 VARCHAR(42),name VARCHAR(20),sex BOOL,telphone CHAR(15), email CHAR(50), authority INTEGER,address VARCHAR(200), endtime TIMESTAMP, context CHAR(100))";
	sqls[6] = "CREATE TABLE reinforce(autoid INTEGER PRIMARY KEY AUTOINCREMENT,code VARCHAR(4) NOT NULL,type VARCHAR(50) NOT NULL,title VARCHAR[100] NOT NULL,status INT(4) NOT NULL DEFAULT(0), primitive TEXT(2048),result INT(4) NOT NULL DEFAULT(0), context TEXT(2048), optime TIMESTAMP DEFAULT (datetime('now','localtime')))";
	for k, v in pairs( sqls ) do
		local dbret = xSQL.execute(mydb,v);
	end
	return string.format("{'action':'setup','result':%d}",0);
end

--导入任务模版
function importModel( jsonstr )
	local model = LoadCSV(".\\model\\model1.csv");
	local pfile = io.open("primitive.json", "r");
	io.input(pfile)
	local jsonstr = io.read("*a")
	io.close(pfile)
	local priData = JSON:decode( jsonstr );	
	for k,v in pairs(model) do
		local priobj = priData[v[3]];
		local pristr = JSON:encode(priobj) or "";
		local sqlstr = string.format("insert into reinforce(type,code,title,primitive) VALUES('%s','%s','%s','%s')",v[2],v[3],v[4],pristr);
		local sqlret = xSQL.execute(mydb, sqlstr );
	end
	return string.format("{'action':'importModel','result':%d}",0);
end


--恢复所有任务
function revertTask()
	local taskList ={};
	local mt = xSQL.prepare(mydb,"SELECT CODE,PRIMITIVE FROM reinforce");
	while xSQL.setup( mt ) == 0 do
		local code = xSQL.column_text(mt,0);
		local prim = xSQL.column_text(mt,1);
		taskList[ code ] = prim;
	end
	xSQL.finalize(mt);
	for k,v in pairs(taskList) do
		TaskA:revert(k,v);
	end
	resetTask()
	return string.format("{'action':'revertTask','result':%d}",0);
end


--重置任务
function resetTask()
	local sql = string.format("update reinforce set result=0,status=0,context=''");
	local ret = xSQL.execute(mydb, sql);
	return string.format("{'action':'resetTask','result':%d}",ret);
end

--获取原始数据
function getPrimitive( code )
	local data ={};
	local mt = xSQL.prepare(mydb,string.format("SELECT PRIMITIVE FROM reinforce WHERE CODE='%s'",code));
	if xSQL.setup( mt ) == 0 then
		local primitive =xSQL.column_text(mt,0);
		data = JSON:decode( primitive );
	end
	xSQL.finalize(mt);
	return data;
end
	

-----------------------------------------------------用户管理-------------------------------------------------------------------------------
function onUserExit()

end


function onUserLogin( josnstr )
	currentUser = "";
end



--------------------------------------------------------启动加及初始信息-----------------------------------------
--网页加载
function onload()
	print("lua onload")
	currentModel = {};
	local mt = xSQL.prepare( mydb, "SELECT TYPE,CODE,TITLE,PRIMITIVE,CONTEXT FROM reinforce");
	while xSQL.setup( mt ) == 0 do
		local worktype = xSQL.column_text(mt,0);;
		if currentModel[worktype] == nil then
			currentModel[worktype] = {};
		end
		local obj = {};
		obj["code"] = xSQL.column_text(mt,1);
		obj["name"] = xSQL.column_text(mt,2);
		obj["primitive"] =xSQL.column_text(mt,3);
		obj["context"] = xSQL.column_text(mt,4) or "";
		obj["status"] = 0; 
		obj["time"] = "";
		obj["argument"] = "";
		obj["scan"]  = false;
		table.insert( currentModel[worktype], obj );
	end
	xSQL.finalize(mt);
	local retstr = JSON:encode(currentModel);
	print( "result onload:" .. retstr );
	return retstr;
end

--设备信息
function macinfo()
	local osname,osversion = SYS.osinfo();
	--local cpufreq, cpuid, cpumid, cputype;-- = SYS.cpuinfo();
	local memtotal,memval = SYS.meminfo();
	local dsktotal,dskval = SYS.diskinfo("/");
	local md5key = MD5.tohex(MD5.sum(string.format("%s%10d","cpumid",132313)));
	local retstr = string.format("{'osname':'%s','osversion':'%s','ontime':'%s','cpufreq':'%d','memtotal':'%f','memval':'%f','dsktotal':'%f','dskval':'%f','prikey':'%s'}",
										osname,osversion,os.date("%Y-%m-%d %H:%M:%S",onStartTime),1500,memtotal,memval,dsktotal,dskval,md5key);
	return retstr;
end

--任务操作记录
function _taskRecord( code, ret, desc, opt )
	local sql = string.format("insert into operatrec(code,type,result,context,user) VALUES('%s',%d, %d,'%s','%s')",code,opt,ret,desc,currentUser);
	local ret = xSQL.execute(mydb, sql);
	print(string.format("taskRecord ret:%d sql:%s",ret,sql));
end

--更新操作状态
function _updateTask( code, ret, desc )
	local sql = string.format("update reinforce set result='%d',context='%s' where code ='%s'",ret,desc,code);
	print( sql );
	local ret = xSQL.execute(mydb, sql);
	print(string.format("updateTask ret:%d sql:%s",ret,sql));
end

--local jsondata = GMSSL.getPeerCert("39.156.66.10",443);
--print( jsondata )

--查询历史记录
function queryHistory( jsonstr )
	local obj = JSON:decode( jsonstr );
	local condition1 = "";
	local condition2 = "";
	if obj.name ~= "不限" then 
		condition1 = string.format("where B.type='%s'",obj.name);
		condition2 = string.format("as A join reinforce as B on a.code = b.code where B.type='%s'", obj.name);
	end
	
	local vcnt = {};
	vcnt.listcount = xSQL.table_count(mydb,"OPERATREC",condition2);
	vcnt.pagecount = math.ceil( vcnt.listcount / 10 );
	vcnt.currentPage = obj.page;
	vcnt.datalist = {};
	local sql = string.format("select A.code, B.type, B.title, A.context, A.user, a.result, a.optime from operatrec as A \
							join reinforce as B on a.code = b.code %s order by a.optime desc LIMIT 10 OFFSET %d",
							condition1,
							obj.page * 10);						
	local mt = xSQL.prepare(mydb,sql);
	while xSQL.setup( mt ) == 0 do
		local item = {};
		item.code = xSQL.column_text(mt, 0);
		item.type = xSQL.column_text(mt, 1);
		item.title = xSQL.column_text(mt, 2);
		item.context = xSQL.column_text(mt, 3);
		item.user = xSQL.column_text(mt,4);
		item.result = xSQL.column_int(mt, 5);
		item.opttime = xSQL.column_text(mt, 6);
		table.insert(vcnt.datalist, item);
	end
	xSQL.finalize(mt);
	return JSON:encode(vcnt);
end

--获取初始设备数据并保存
function collectPrimitive()
	print("collectPrimitive")
	local codeList = {};
	local mt = xSQL.prepare( mydb, "SELECT CODE FROM REINFORCE");
	while xSQL.setup( mt ) == 0 do
		local code = xSQL.column_text(mt,0);
		table.insert( codeList, code );
	end
	xSQL.finalize(mt);
	local primidata = {};
	for _,v in pairs(codeList) do
		local data = TaskA:query( v );
		primidata[v] = data;
		local sql = string.format("UPDATE REINFORCE SET PRIMITIVE='%s' WHERE CODE='%s'",JSON:encode(data),v);
		local ret = xSQL.execute(mydb, sql);		
	end
	local filename = "primitive.json"
	if io.open(filename,"r") == nil then
		local file = io.open(filename, "a");
		io.output(file);
		io.write(JSON:encode(primidata));
		io.close(file);
	end
end

--执行任务
function executeTask( jsonstr )
	local obj  = JSON:decode( jsonstr );
	local data = getPrimitive(obj.code);
	local ret,ctx = TaskA:execute(obj.code,data);
	local sctx = JSON:encode( ctx );
	_taskRecord( obj.code,ret,sctx,2 );
	_updateTask( obj.code,ret,sctx );
	return string.format("{'result':%d,'code':'%s','context':'%s'}",ret,obj.code,sctx);
end

--查询任务
function queryTask( jsonstr )
	local obj  = JSON:decode( jsonstr );
	local ctx  = TaskA:query( obj.code );
	local sctx = JSON:encode( ctx );
	return string.format("{'code':'%s','context':'%s'}",obj.code,sctx);
end

--恢复任务
function revertTask( jsonstr )
	print( "revertTask:" .. jsonstr );
	local obj  = JSON:decode( jsonstr );
	local data = getPrimitive( obj.code );
	local ret  = TaskA:revert( obj.code, data);
	return string.format("{'result':%d,'code':'%s'}",ret,obj.code);
end

--启动
function startup()
	mydb = xSQL.open("./Reinforce.db","czy20101204");
	collectPrimitive();
	print( mydb );
end

--关闭
function shutdown()
	xSQL.close( mydb );
end

--LUA脚本宿主,安装事件处理函数
mainProc = coroutine.create(function(t)install(t,
	function( addr, data, len )
		print("trans...\n");
		funcode = string.byte(string.sub(data,1,1));
		tagname = string.sub(data,2,len);
	end,
    function( sn )
		--print("timer...\n");
	end,
	function()
		print("close...\n");
		print("lua mainproc exit!");
	end)
end)

startup();
coroutine.resume(mainProc,1000);
print("start infinalize")






