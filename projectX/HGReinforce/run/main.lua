--*****LUA SERMON SCRIPT*****--
print("start HG project at:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local JSON   = require "luajson"
local CSV    = require "luacsv"
local XAPI   = require "luaxapi"
local MD5    = require "luamd5"
local BITX   = require "luabit"
local OPENCV = require "opencv"
local SYS    = require "system"
local SQLITE = require "sqlite"

require("main_alc")
require("main_sec")
require("main_ips")

local currentModel = {};
local currentTable = {};
local scanPos = 0;
local scanStatus = 0;

local mydb = 0;

------------------------------------------------------恢复出厂设置-------------------------------------------------------------------------------
function factorysetup(name,account,desc )
	if os.getenv("OS") == "Windows_NT" then
		os.execute("rmdir /s/q logs");
		os.execute("rmdir /s/q data");
	else
		os.execute("rm -r -f ./logs");
		os.execute("rm -r -f ./data");
	end
	local sqls={};
	sqls[1] = "DELETE FROM channels";
	sqls[2] = "UPDATE sqlite_sequence SET seq=0 WHERE name='channels'";
	sqls[3] = "DELETE FROM chndetect";
	sqls[4] = "UPDATE sqlite_sequence SET seq=0 WHERE name='chndetect'";
	sqls[5] = "DELETE FROM pointinfo";
	sqls[6] = "UPDATE sqlite_sequence SET seq=0 WHERE name='pointinfo'";
	for k, v in pairs( sqls ) do
		local dbret = SQLITE.execute(mydb,v);
	end
	local strsql= string.format("replace into device(idx,name,setuptime,setupuser,desc) VALUES(0,'%s',%d,'%s','%s')",name,os.time(),account,desc);
	local dbret = SQLITE.execute(mydb, strsql);
	local jsret = string.format("{'action':'setup','result':%d}",dbret);
	return jsret;
end



--------------------------------------------------------启动加及初始信息-----------------------------------------
--网页加载
function onload()
	print("lua onload")
	currentModel = {};
	currentTable = LoadCSV(".\\model\\model1.csv");
	for k,v in pairs(currentTable) do
		local worktype = v[2];
		if currentModel[worktype] == nil then
			currentModel[worktype] = {};
		end
		local obj = {};
		obj["code"]  = v[3];
		obj["name"]  = v[4];
		obj["context"]  = _queryStatus( v[3] );
		obj["status"]  = 0; 
		obj["time"]  = v[7] or "";
		obj["argument"]  = "";
		obj["scan"]  = false;
		table.insert( currentModel[worktype], obj );
	end
	local retstr = JSON:encode(currentModel);
	print( "result onload:" .. retstr );
	--OPENCV.screenshot("./now.jpg")
	return retstr;
end

--设备信息
function macinfo()
	local osname,osversion = SYS.osinfo();
	--local cpufreq, cpuid, cpumid, cputype;-- = SYS.cpuinfo();
	local memtotal,memval = SYS.meminfo();
	local dsktotal,dskval = SYS.diskinfo("/");
	local md5key = MD5.tohex(MD5.sum(string.format("%s%10d","cpumid",132313)));
	local retstr = string.format("{'osname':'%s','osversion':'%s','ontime':'%s','cpufreq':'%d','cpumid':'%s','cputype':'%s','memtotal':'%f','memval':'%f','dsktotal':'%f','dskval':'%f','prikey':'%s'}",
										osname,osversion,os.date("%Y-%m-%d %H:%M:%S",onStartTime),1500,"cpumid","AMD",memtotal,memval,dsktotal,dskval,md5key);
	return retstr;
end

--网页关闭
function unload()

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

--local jsondata = GMSSL.getPeerCert("39.156.66.10",443);
--print( jsondata )

--状态查询
function _queryStatus( code )
	local ret = 0;
	local context = "";
	if     code == "A1"  then ret,context = A1(0)
	elseif code == "A2"  then ret,context = A2(0);
	elseif code == "A3"  then ret,context = A3(0);
	elseif code == "A4"  then ret,context = A4(0);
	elseif code == "A5"  then ret,context = A5(0);
	elseif code == "A6"  then ret,context = A6(0);	
	elseif code == "A7"  then ret,context = A7(0);
	elseif code == "A8"  then ret,context = A8(0);
	elseif code == "A9"  then ret,context = A9(0);
	elseif code == "A10" then ret,context = A10(0);
	elseif code == "A11" then ret,context = A11(0);
	elseif code == "A12" then ret,context = A12(0);
	elseif code == "A13" then ret,context = A13(0);
	elseif code == "A14" then ret,context = A14(0);
	elseif code == "A15" then ret,context = A15(0);
	elseif code == "A16" then ret,context = A16(0);	
	elseif code == "A17" then ret,context = A17(0);
	elseif code == "A18" then ret,context = A18(0);
	elseif code == "A19" then ret,context = A19(0);
	elseif code == "A20" then ret,context = A20(0);
	elseif code == "A21" then ret,context = A21(0);
	elseif code == "A22" then ret,context = A22(0);
	elseif code == "A23" then ret,context = A23(0);
	elseif code == "A24" then ret,context = A24(0);
	elseif code == "A25" then ret,context = A25(0);	
	elseif code == "A26" then ret,context = A26(0);
	elseif code == "A27" then ret,context = A27(0);
	end
	return context;
end

--执行任务
function startTask( jsonstr )
	local retObj = "{'retcode':-1}";
	local obj = JSON:decode( jsonstr );
	XAPI:PrintTable( obj );
	local result, context;
	if obj.code == "A1"      then result,context = A1(2);
	elseif obj.code == "A2"  then result,context = A2(2);
	elseif obj.code == "A3"  then result,context = A3(2);
	elseif obj.code == "A4"  then result,context = A4(2);
	elseif obj.code == "A5"  then result,context = A5(2);
	elseif obj.code == "A6"  then result,context = A6(2);
	elseif obj.code == "A7"  then result,context = A7(2);
	elseif obj.code == "A8"  then result,context = A8(2);
	elseif obj.code == "A9"  then result,context = A9(2);
	elseif obj.code == "A10" then result,context = A10(2);
	elseif obj.code == "A11" then result,context = A11(2);
	elseif obj.code == "A12" then result,context = A12(2);
	elseif obj.code == "A13" then result,context = A13(2);
	elseif obj.code == "A14" then result,context = A14(2);
	elseif obj.code == "A15" then result,context = A15(2);
	elseif obj.code == "A16" then result,context = A16(2);
	elseif obj.code == "A17" then result,context = A17(2);
	elseif obj.code == "A18" then result,context = A18(2);
	elseif obj.code == "A19" then result,context = A19(2);
	elseif obj.code == "A20" then result,context = A20(2);
	elseif obj.code == "A21" then result,context = A21(2);
	elseif obj.code == "A22" then result,context = A22(2);
	elseif obj.code == "A23" then result,context = A23(2);
	elseif obj.code == "A24" then result,context = A24(2);
	elseif obj.code == "A25" then result,context = A25(2);
	elseif obj.code == "A26" then result,context = A26(2);
	elseif obj.code == "A27" then result,context = A27(2);
	end
	return string.format("{'result':%d,'code':'%s','context':'%s'}", result, obj.code, context);
end

function A100()
	return 0,"ok"
end

--恢复所有任务
function revertTask()
	A1( 1 );
	A2( 1 );
	A3( 1 );
	A4( 1 );
	A5( 1 );
	A6( 1 );
	A7( 1 );
	A8( 1 );
	A9( 1 );
	A10( 1 );
	A11( 1 );
	A12( 1 );
	A13( true );
	A14( true );
	A15( true );
	A16( true );
	A17( true );
	A18( true );
	A19( true );
	A20( true );
end



--启动
function startup()
	mydb = SQLITE.open("Reinforce.db","czy20101204");
	--载入信息
	local mt = SQLITE.prepare( mydb, string.format("select name, driver, user, password from dnsinfo"));
	while SQLITE.setup( mt ) == 0 do
		local dnsobj = {};
		local name = SQLITE.column_text(mt,0);
		dnsobj.driver  = SQLITE.column_text(mt,1);
		dnsobj.user = SQLITE.column_text(mt,2);
		dnsobj.password  = SQLITE.column_text(mt,3);
		dnsobj.handle = nil;
		mydns[name] = dnsobj;
	end
	SQLITE.finalize(mt);
end


coroutine.resume(mainProc,1000);
print("start infinalize")


--A2(2);
--A2(0);

--local retStr = REGEDIT.cmdPopen("dir");
--print( retStr )

--doMainTask();

--OPENCV.screenshot("./now.jpg")

--startup();


print("start infinalize")



