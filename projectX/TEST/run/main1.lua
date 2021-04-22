--**LUA SERMON SCRIPT*****--
print("start verity project at:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local JSON  = require "luajson"
local SYS   = require "system"
local SQLITE  = require "sqlite"
local ODBC  = require "odbc"

--protocal
local DLT104  = require "protocal/DLT104"
local DLTZGXH = require "protocal/DLTZGXH"
local JKQ011  = require "protocal/JKQ011"
local CDT91   = require "protocal/CDT91"

local mydb = 0;
local myModules={};
local onStartTime = os.time();

local sermonobj;
local websockobj;

local db1 = nil;
local db2 = nil;

local mydns={};
local cmptblinfo = {};    --当前比对信息

local stationArray={};



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
---------------------------------------------------device info------------------------------------------
function macinfo()
	local osname,osversion = SYS.osinfo();
	local cpufreq, cpuid, cpumid, cputype = SYS.cpuinfo();
	local memtotal,memval = SYS.meminfo();
	local dsktotal,dskval = SYS.diskinfo("/");
	local md5key = MD5.tohex(MD5.sum(string.format("%s%10d",cpumid,cpuid)));
	local retstr = string.format("{'osname':'%s','osversion':'%s','ontime':'%s','cpufreq':'%d','cpumid':'%s','cputype':'%s','memtotal':'%f','memval':'%f','dsktotal':'%f','dskval':'%f','prikey':'%s'}",
									osname,osversion,os.date("%Y-%m-%d %H:%M:%S",onStartTime),cpufreq,cpumid,cputype,memtotal,memval,dsktotal,dskval,md5key);
	return retstr;
end

function onload()
--[[	local localip = websockobj.getip("\\Device\\NPF_{8219401C-0BEC-4493-B3DC-14737E7848E2}");
	print( "ipaddr:" ..  localip );
	local retjson = "{'stations':[";
	local mt = SQLITE.prepare( mydb, string.format("select sid,mark,name,level,splite from stations"));
	local hasdata = false;
	while SQLITE.setup( mt ) == 0 do
		local id = SQLITE.column_int(mt,0);
		local mk = SQLITE.column_text(mt,1);
		local na = SQLITE.column_text(mt,2);
		local le = SQLITE.column_text(mt,3);
		local sp = SQLITE.column_text(mt,4);
		retjson = retjson .. string.format("{'id':%d,'mark':'%s','name':'%s','level':'%s','splite':'%s'},",id,mk,na,le,sp);
		stationArray[id] = mk;
		hasdata = true;
	end
	SQLITE.finalize(mt);--]]
	if hasdata then	
		regjson = string.sub(retjson,1,-2);
	end
	retjson = retjson.. "], \"svrip\":\"" .. localip .. "\"}";
	print( retjson );
	return retjson;
end

---------------------------------------------------user manager-------------------------------------------------------------------------------
--
function getuserlist()
	local retjson = "{'users':[";
	local mt = SQLITE.prepare(mydb, "select account,name,authority,telphone,email,endtime from users where authority<128");
	while SQLITE.setup( mt ) == 0 do
		local account = SQLITE.column_text( mt, 0);
		local name = SQLITE.column_text( mt, 1 );
		local authority = SQLITE.column_int( mt, 2 );
		local telphone = SQLITE.column_text( mt, 3 );
		local email = SQLITE.column_text( mt, 4 );
		local endtime = SQLITE.column_text( mt, 5 );
		retjson = retjson .. string.format("{'account':'%s','name':'%s','authority':%d,'telphone':'%s','email':'%s','endtime':'%s'},",
											account,name,authority,telphone,email,endtime);
	end
	SQLITE.finalize(mt);
	retjson = string.sub(retjson,1,-2) .. "]}";
	return retjson;
end
--usermgr--
function userlogin( acc, pwd )
	local username = "";
	local authority = 0;
	local result = -1;
	local mt = SQLITE.prepare(mydb,string.format("select name,authority from users where account='%s' and pwdmd5='%s'",acc,pwd));
	if SQLITE.setup( mt ) == 0 then
		username  = SQLITE.column_text( mt, 0 );
		authority = SQLITE.column_int( mt, 1 );
		result = 0;
	end
	-- 1：user  2：admin  4: system 8:administrator
	print( "authority:"  .. authority );
	SQLITE.finalize(mt);
	if result == 0 then
		local strsql = string.format("update users set endtime='%s' where account='%s'",os.date("%Y-%m-%d %H:%M:%S", os.time()),acc);
		local retsql = SQLITE.execute(mydb, strsql);
	end
	local retstr = string.format("{'action':'login','result':%d,'name':'%s','authority':%d}",result,username,authority);
	return retstr;
end
function userlogout()
	sermonobj.printrealdata('');
	sermonobj.printhistorydata('');
	return string.format("{'action':'logout','result':0}");
end
--checkchn
function checkuseraccount( account )
	local userExist = 0;
	local mt = SQLITE.prepare(mydb, string.format("select * from users where account='%s'",account));
	if SQLITE.setup( mt ) == 0 then
		userExist = 1;
	end
	SQLITE.finalize(mt);
	return string.format("{'action':'chkaccount','result':%d}",userExist);
end
--create user
function createuser( account,name,sex,authority,address,telphone,email )
	local pwdmd5 =  MD5.tohex(MD5.sum("20101204"));
	local sql = string.format("insert into users(account,pwdmd5,name,sex,authority,address,telphone,email) VALUES('%s','%s','%s',%d,%d,'%s','%s','%s')",
							    account,pwdmd5,name,sex,authority,address,telphone,email);
	local ret = SQLITE.execute(mydb, sql );
	local retstr = string.format("{'action':'crtchn','result':%d}",0);
	return retstr;
end
--removeUser
function removeUser( account )
	local sql = string.format("delete from users where account='%s'",account);
	local ret = SQLITE.execute(mydb, sql );
	return string.format("{'action':'removeuser','result':%d}", ret);
end
--getuserinfo--
function getuserinfo( account )
	local username = "";
	local telphone = "";
	local email = "";
	local address="";
	local context = "";
	local mt = SQLITE.prepare(mydb,string.format("select name,telphone,email,address,context from users where account='%s'",account));
	if SQLITE.setup( mt ) == 0 then
		username = SQLITE.column_text( mt, 0 );
		telphone = SQLITE.column_text( mt, 1 );
		email = SQLITE.column_text( mt, 2 );
		address = SQLITE.column_text( mt, 3 );
		context = SQLITE.column_text( mt, 4 );
	end
	SQLITE.finalize(mt);
	local retstr = string.format("{'action':'userinfo','result':0,'name':'%s','telphone':'%s','email':'%s','address':'%s','context':'%s'}",
							username,telphone,email,address,context);
	return retstr;
end
--updateuserinfo--
function updateuserinfo( acc,name,pwd,tel,email,addr )
	local pwdmd5 =  MD5.tohex(MD5.sum(pwd));
	local strsql = string.format("update users set name='%s',pwdmd5='%s',telphone='%s',email='%s',address='%s' where account='%s'",name,pwdmd5,tel,email,addr,acc);
	local retsql = SQLITE.execute(mydb, strsql);
	local retstr = string.format("{'action':'setuserinfo','result':%d}",retsql);
	return retstr;
end

----------------------------------------------station manager--------------------------------------------------------------
function getstationlist( )
	print("getstationlist");
	local retjson = "{'stations':[";
	local mt = SQLITE.prepare( mydb, string.format("select sid,mark,name,style,level,splite from stations order by level"));
	local hasdata = false;
	while SQLITE.setup( mt ) == 0 do
		local sid  = SQLITE.column_int(mt,0);		
		local mark  = SQLITE.column_text(mt,1);
		local name  = SQLITE.column_text(mt,2);
		local style = SQLITE.column_text(mt,3);
		local level = SQLITE.column_text(mt,4);
		local split = SQLITE.column_text(mt,5);
		retjson = retjson .. string.format("{'sid':%d,'mark':'%s','name':'%s','tyle':'%s','level':'%s','splite':'%s'},",sid,mark,name,style,level,split);
		hasdata = true;
	end
	SQLITE.finalize(mt);
	if hasdata == true then
		retjson = string.sub(retjson,1,-2);
	end
	retjson = retjson .. "]}";
	print( retjson );
	return retjson;
end

function checkstation( sid )
	local stationExist = 0;
	local mt = SQLITE.prepare( mydb, string.format("select sid from stations where sid=%d", sid));
	if SQLITE.setup( mt ) == 0  then
		stationExist = 1;
	end
	SQLITE.finalize(mt);
	return string.format("{'action':'chkstation','result':%d}",stationExist);
end

function getstationinfo( sid )
	print( "getstationinfo:"..id);
	local retjson = "{'stations':[";
	local mt = SQLITE.prepare( mydb, string.format("select sid,mark,name,style,level from stations where sid=%d", sid));
	if SQLITE.setup( mt ) == 0 then
		local sid  = SQLITE.column_int(mt,0);		
		local mark  = SQLITE.column_text(mt,1);
		local name  = SQLITE.column_text(mt,2);
		local style = SQLITE.column_text(mt,3);
		local level = SQLITE.column_text(mt,4);
		retjson = string.format("{'sid':%d,'mark':'%s','name':'%s','tyle':'%s','level':'%s'}",sid,mark,name,style,level);
	end
	SQLITE.finalize(mt);
	return retjson;
end


--Create Station
function createstation(sid,mark,name,level,style,splite)
	print("createstation");
	local sql = string.format("insert into stations(sid,mark,name,level,style,splite) VALUES(%d,'%s','%s','%s','%s','%s')",sid,mark,name,level,style,splite);
	print( sql );
	local ret = SQLITE.execute(mydb, sql );
	local retjson = string.format("{'action':'crtstation','result':%d}",0);
	return retjson;
end

------------------------------------------------- point manager ------------------------------------------------------------------------------
function getpointlist( sid, type )
	local retjson = "{'sql':[";
	local sql1 = "";
	local sql2 = "";
	local mtx = SQLITE.prepare( mydb, string.format("select datasrc,datacmp from cmpinfo where sid=%d and project='%s'",sid,type));
	if SQLITE.setup( mtx ) == 0 then
		sql1 = SQLITE.column_text(mtx,0);
		sql2 = SQLITE.column_text(mtx,1);
		retjson = retjson .. "'" .. sql1 .. "'," .."'" .. sql2 .. "'";
	end
	SQLITE.finalize(mtx);
	retjson = retjson .. "],";
	retjson = retjson .. "'points':[";

	--获取第一份数据

	local dns1  = mydns["veritydns"];
	local hsmt1 = ODBC.execute( dns1, "select id,name,val from yxtbl" );
	local cols1 = ODBC.NumResultCols( hsmt1 );
	while( ODBC.Fetch(hsmt1) == 0 ) do

		print( ODBC.GetData(hsmt1,2) );
		for i = 1,cols1 do
			print( ODBC.GetData(hsmt1,i) );
		end
	end
	ODBC.freeHandle(hsmt1);

	local mt = SQLITE.prepare( mydb, string.format("select pn,desc,val,tms from yx"));
	while SQLITE.setup( mt ) == 0 do
		local pn   = SQLITE.column_int(mt,0);
		local desc = SQLITE.column_text(mt,1);
		local val  = SQLITE.column_int(mt,2);
		local tms  = SQLITE.column_text(mt,3);
		retjson = retjson .. string.format("{'pn':%d,'desc':'%s','val':%d,'tms':'%s'},",pn,desc,val,tms);
	end
	SQLITE.finalize(mt);
	retjson = string.sub(retjson,1,-2) .. "]}";
	print( retjson );
	return retjson;
end
function insertpoint( chnname, ctl, id, desc )
	local sqlstr = string.format("insert into pointinfo(chnname,control,pointid,desc) VALUES('%s','%s',%d,'%s')",chnname,ctl,id,desc);
	local sqlret = SQLITE.execute(mydb, sqlstr );
	local luaret = string.format("{'action':'insertpoint','result':%d}",sqlret);
	pointinfolist[id] = desc;
	return luaret;
end
function resetpoint( sid, subject )
	local sqlstr = string.format("update %s set val=0", subject);
	local sqlret = SQLITE.execute(mydb, sqlstr );
	local luaret = string.format("{'action':'removepoint','result':%d}",sqlret);
	return luaret;
end

--更新YX表信息
function updateYXfromupfile( file )
	local dtbl = CSV:loadcsv( "./webui/upload/" .. file );
	for k, v in pairs( dtbl ) do
		local strsql= string.format("replace into yx(pn,desc) VALUES(%d,'%s')",v[1],v[2]);
		local dbret = SQLITE.execute(mydb, strsql);
	end
end

----------------------------------------------- databases info  --------------------------------------------------------------------
local cmptblinfo={};

function getdnsinfo()
	print("getdnsinfo");
	local mt = SQLITE.prepare( mydb, string.format("select name, driver, user, password from dnsinfo"));
	while SQLITE.setup( mt ) == 0 do
		local name = SQLITE.column_int(mt,0);
		local dns  = SQLITE.column_text(mt,1);
		local user = SQLITE.column_int(mt,2);
		local pwd  = SQLITE.column_text(mt,3);
		mydns[ name ] = ODBC.connect(dns,user,pwd);
		print( name .. dns .. user .. pwd);
	end
	SQLITE.finalize(mt);
end


--数据查询
function dataquery( station, subject )
	cmptblinfo.type = subject;
	cmptblinfo.header = header;
	cmptblinfo.item = {};

	--第一份数据
	local datasourceA = "";
	local datasourceB = "";
	local mt = SQLITE.prepare( mydb, string.format("select name, driver, user, password from dnsinfo"));
	if SQLITE.setup( mt ) == 0 then
		datasourceA = SQLITE.column_int(mt,0);
		datasourceB = SQLITE.column_text(mt,1);
	end
	SQLITE.finalize(mt);
	--
	local datasourceAtype = "";
	local datasourceBtype = "";
	local datasourceAname = "";
	local datasourceBname = "";
	local datasourceAdesc = "";
	local datasourceBdesc = "";

	--
	local dns1  = mydns[ datasourceAname ];
	local hsmt1 = ODBC.execute( dns1, datasourceAdesc );
	local cols1 = ODBC.NumResultCols( hsmt1 )
	while( ODBC.Fetch(hsmt1) == 0 ) do
		local line = ODBC.GetData( hsmt1,1); --
		cmptblinfo.item[line] = {};
		cmptblinfo.item[line].mark = line
		cmptblinfo.item[line].data = {}
		cmptblinfo.item[line].meger = 0;
		cmptblinfo.item[line].desc = ""
		cmptblinfo.item[line].operator = 0
		for i = 2,cols1 do
			table.insert(cmptblinfo.item[line].data, odbc.GetData(hsmt1,i));
		end
	end
	ODBC.freeHandle(hsmt1)

	--第二份数据
	local dns2  = mydns[ datasourceBname ];
	local hsmt2 = ODBC.execute( dns2, datasourceBdesc );
	local cols2 = ODBC.NumResultCols( hsmt2 )
	while( ODBC.Fetch(hsmt2) == 0 ) do
		local line = ODBC.GetData( hsmt2,1); --
		cmptblinfo.item[line] = {};
		cmptblinfo.item[line].mark = line
		cmptblinfo.item[line].data = {}
		cmptblinfo.item[line].meger = 0;
		cmptblinfo.item[line].desc = ""
		cmptblinfo.item[line].operator = 0
		for i = 2,cols2 do
			table.insert(cmptblinfo.item[line].data, odbc.GetData(hsmt2,i));
		end
	end
	ODBC.freeHandle(hsmt2)


	--数据
	sheader = ""
	sdata = ""
 	for k,v in pairs(my_stationtbl.header) do
		sheader = sheader .. "\"" .. v .."\","
 	end

 	local tmtst={};
 	for k, v in pairs(my_stationtbl.item) do
 		tmtst[ tonumber(k) + 1 ] = v
 	end

 	for k,v in pairs(tmtst) do
		item = "{\"mark\": \"" .. v.mark .. "\", \"data\":[\"" .. v.mark .."\","
		for x, y in pairs(v.data ) do
			item = item .. "\"" .. y .."\","
		end
		item = string.sub(item, 1, -2)
		sdata = sdata .. item .. "]},"
 	end
	sheader = string.sub(sheader, 1, -2)
	sdata   = string.sub(sdata, 1, -2)
	return string.format("{\"status\":\"%d\",\"src1\":\"%s\",\"src2\":\"%s\",\"tbhead\":[%s],\"tbdata\":[%s]}",0,string.format(sql1,sinfo[1].id),string.format(sql2,sinfo[2].id),sheader,sdata)
end


--数据比较
function datameger( items )
	--cmptblinfo;
end

--更新当前查询信息
function updatequeryinfo( sid, subject, sql1, sql2 )
	sql1 = string.gsub(sql1,'~', ',');
	sql2 = string.gsub(sql2,'~', ',');
	print( subject .. sql1 .. sql2 );
	local strsql= string.format("replace into cmpinfo(id,project,datasrc,datacmp) VALUES(%d,'%s','%s','%s')",sid, subject, sql1,sql2);
	local dbret = SQLITE.execute(mydb, strsql);
	local retjson = "{'points':[";
	local mt = SQLITE.prepare( mydb, string.format("select pn,desc,val,tms from yx"));
	while SQLITE.setup( mt ) == 0 do
		local pn   = SQLITE.column_int(mt,0);
		local desc = SQLITE.column_text(mt,1);
		local val  = SQLITE.column_int(mt,2);
		local tms  = SQLITE.column_text(mt,3);
		retjson = retjson .. string.format("{'pn':%d,'desc':'%s','val':%d,'tms':'%s'},",pn,desc,val,tms);
	end
	SQLITE.finalize(mt);
	retjson = string.sub(retjson,1,-2) .. "]}";
	print( retjson );
	return retjson;
end

-----------------------------------------------协议处理-------------------------------------------------------------------
function process104( tag, jsontext )
	print( tag .. ":process104");
	local tbs = JSON:decode(jsontext);
	XAPI:PrintTable(tbs);
	if tbs.PNTS == nil then
		return;
	end
	for k, v in pairs( tbs.PNTS ) do
		--local strsql= string.format("replace into yx(pn,val,tms) VALUES(%d,%d,'%s')",v[1], v[2], os.date("%Y-%m-%d %H:%M:%S", os.time()));
		--local dbret = SQLITE.execute(mydb, strsql);
	end
	nsendlen = websockobj.sendstr(retjson);
	print(retjson)
end


function process101( tag, jsontext )



end

-----------------------------------------------报文处理------------------------------------------------------------------------------
--报文校验
function frameCheksum(protocal,data,len)
	if protocal == "DLTZGXH" then
		return DLTZGXH:protocal_checksum( data, len );
	elseif protocal == "JKQ011" then
		return JKQ011:protocal_checksum( data, len );
	elseif protocal == "CDT91" then
		return CDT91:protocal_checksum( data, len );
	end
	return false
end

--组帧
function frameCombine(protocal, addr)
	local data;
	local len = 0;
	if protocal == "DLTZGXH" then
		data, len = DLTZGXH:framesimulate( addr )
	elseif protocal == "JKQ011" then
		data, len = JKQ011:framesimulate( addr )
	elseif protocal == "CDT91" then
		data, len = CDT91:framesimulate( addr )
	elseif protocal == "DLT104" then
		data, len = DLT104:framesimulate( addr )
	end
	return data,len;
end

--报文简述
function frameResume(tag,data,len)
	local retcode,context;
	if myModules[tag].protocal == "DLTZGXH" then
		retcode,context = DLTZGXH:framecheck(data,len);
	elseif myModules[tag].protocal == "JKQ011" then
		retcode,context = JKQ011:framecheck(data,len);
	elseif myModules[tag].protocal == "CDT91" then
		retcode,context = CDT91:framecheck(data,len);
	elseif myModules[tag].protocal == "DLT104" then
		retcode,context = DLT104:framecheck(data,len);
	end
	return retcode,context;
end

--HEX 字符串报文解析
function frameExplain( protocal, hexstr )
	print("frameExplain:" .. hexstr)
	local data = XAPI:hex2bin(hexstr);
	local luastr,pntdesc;
	local result,addr,explain;
	addr = -1;
	pntdesc = "";
	explain = "";
	if protocal == "DLTZGXH" then
		result,addr,explain = DLTZGXH:frameanalyse(data,string.len(data));
	elseif protocal == "JKQ011" then
		result,addr,explain = JKQ011:frameanalyse(data,string.len(data));
	elseif protocal == "CDT91" then
		result,addr,explain = JKQ011:frameanalyse(data,string.len(data));
	elseif protocal == "DLT104" then
		result,addr,explain = DLT104:frameanalyse(data,string.len(data));
	end
	luastr = string.format("{'RESULT':%d,'ADDR':%d,%s}", result,addr,explain);
	process104("abc", "{".. explain .. "}");
	return luastr;
end

--------------------------------------------------------------系统--------------------------------------------------------------------------------------

--[实时]数据解析处理
function realdataprocess( tag, data )
	print(tag .. "  "  .. XAPI:bin2hex(data));
	local retcode,context;
	if myModules[tag].protocal == "DLTZGXH" then
		retcode,context = DLTZGXH:framecheck(data,len);
	elseif myModules[tag].protocal == "JKQ011" then
		retcode,context = JKQ011:framecheck(data,len);
	elseif myModules[tag].protocal == "CDT91" then
		retcode,context = CDT91:framecheck(data,len);
	elseif myModules[tag].protocal == "DLT104" then
		 result,addr,explain = DLT104:frameanalyse(data,string.len(data));
		 print( result );
		 print( addr );
		 print( explain );
		 process104(tag, explain);
		 websockobj.sendstr( explain );
	end
end

--启动
function startup()
	mydb = SQLITE.open("verity.db","czy20101204");
	local netcard={};
	if os.getenv("OS") == "Windows_NT" then
		--netcard["first"]  = "\\Device\\NPF_{95D04454-325F-4EB3-99C2-F428D36EABEF}";
		--netcard["first"] = "\\Device\\NPF_{A4889D0F-C5B2-49CE-85C5-D05C13152CEA}";
		--netcard["third"]  = "\\Device\\NPF_{BE482400-01D3-41A9-8DD0-DA4F9FCDC431}";
		--netcard["HGTEST"]  = "\\Device\\NPF_{423DABCE-EAE6-405F-B2A9-5A53BF40F776}";
	else
		netcard["first"]  = "eth0";
		--netcard["second"]  = "bluetooth0";
		--netcard["third"]  = "nflog";
		--netcard["forth"]  = "nfqueue";
		--netcard["fifth"]  = "usbmon1";
		--netcard["sixth"]  = "usbmon2";
	end

	for k, v in pairs( netcard ) do
		myModules[k] = {};
		myModules[k].caption = "sdafsdf";
		myModules[k].chntype = 7;
		--myModules[k].params = v .. ",port 80";
		--myModules[k].protocal = "104";

		myModules[k].params = v .. ",port 8006";
		myModules[k].protocal = "DLT104";

		myModules[k].storage = "./data/" .. k;
		myModules[k].lasttick = 0;
		myModules[k].status  = 0;
		myModules[k].recvcnt = 0;
		myModules[k].sendcnt = 0;
		myModules[k].tigger = 0;
		myModules[k].chn={};
		myModules[k].chn[1]={};
		myModules[k].chn[2]={};
		myModules[k].chn[1].bytes = 0;
		myModules[k].chn[2].bytes = 0;
		myModules[k].chn[1].frames = 0;
		myModules[k].chn[2].frames = 0;
		myModules[k].chn[1].vaild = 0;
		myModules[k].chn[2].vaild = 0;
	end

	--载入dns信息
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

--关闭
function shutdown()
	SQLITE.close( mydb );
end

function testfunction( arg1, arg2 )
	print("testfunction:" .. hexstr)
	return string.format("{'arg1':'%s','arg2':'%s'}", arg1,arg2);
end

--加载比对历史记录
function showMegerHistory( stationname, datatype )
	print("ShowMegerHistory name:"..stationname.." type:"..datatype);
	local sj = ""
	local sd = string.format("./data/%s/%s/*",stationname,datatype);
	local vd = SYS.ListDirFile( sd );
	for j = 1, #vd, 1 do
		local sf = ""
		local vf = SYS.ListDirFile( string.format("./data/%s/%s/%s/*",stationname,datatype,vd[j]));
		for k = 0, #vf, 1 do
			sf = sf .. "\"" .. vf[k] .."\",";
		end
		sf = string.sub(sf,1,-2);
		sj = sj .."{\"dir\":\"" .. vd[j] .. "\",\"files\":[" .. sf .. "]},";
	end
	sj = string.sub( sj, 1, -2);
	return string.format("[%s]",sj);
end

function showstationhistory( station )
	local sj = ""
	local sd = "./data/" .. station .. "/static/*";
	local vd = SYS.ListDirFile( sd );
	for j = 1, #vd, 1 do
		local sf = ""
		local vf = SYS.ListDirFile( string.format("./data/%s/static/%s/*",station,vd[j]));
		for k = 0, #vf, 1 do
			sf = sf .. "\"" .. vf[k] .."\",";
		end
		sf = string.sub(sf,1,-2);
		sj = sj .."{\"dir\":\"" .. vd[j] .. "\",\"files\":[" .. sf .. "]},";
	end
	sj = string.sub( sj, 1, -2);
	return string.format("[%s]",sj);
end

--save meger data
function savemegerhistory( sid, tid, data  )
	print( "savemegerhistory:" .. sid .. "  tid:" ..tid );
	local staname  =  stationArray[tonumber(sid)] ;
	local dirname  = os.date("%Y-%m-%d", os.time());
	local filename = os.date("%H-%M-%S.sm", os.time());
	local pathname = string.format("./data/%s/static/%s/%s", staname, tid, dirname);
	local lastname =  pathname .. "/" .. filename;
	SYS.CreatePath( pathname );
	local f = assert(io.open(lastname, 'w+b'));
	local data = string.gsub( data,'|', ',');
	f:write( data );
	f:close();
	return string.format("{\"dir\":\"%s\",\"filename\":\"%s\"}", dirname, filename );
end

function loadmegerhistory( stationid, src )
	print("loadmegerhistory:"..stationid.." src:"..src );
	local staname =  stationArray[tonumber(stationid)] ;
	local spath = string.format("./data/%s/static/%s", staname, src );
	local file = io.open(spath, "rb");
	local sdata = "";
	if file then 
		sdata = file:read("*all");
		file:close();
	end
	return sdata;
end

function removemegerhistory( stationid, src )
	print("removemegerhistory sid:"..stationid.." src:"..src );
	local staname =  stationArray[tonumber(stationid)] ;
	local delfile = string.format("./data/%s/static/%s",staname,src);
	local ret = SYS.removefile(delfile);
	return string.format("{'ret':%d}",ret);
end
-----------------------------------------------------------------------------
function odbcQuery( dns, sql )
	print("odbcQuery dns:" .. dns .. " sql:" .. sql);
	local dnsobj = mydns[dns];
	if dnsobj == nil then
		return string.format("ret:-1");
	end
	if dnsobj.handle == nil then
		print("dnsinfo:"..dnsobj.driver.."  user:"..dnsobj.user.."  password:"..dnsobj.password);
		dnsobj.handle = ODBC.connect(dnsobj.driver,dnsobj.user,dnsobj.password);
	end
	local hsmt1 = ODBC.execute( dnsobj.handle, string.gsub(sql,'|', ',')); 
	local cols1 = ODBC.NumResultCols( hsmt1 );
	local shdr = "";
	for i = 1,cols1 do
		 shdr = shdr.."\""..ODBC.GetTitle(hsmt1, i).."\","
	end
	shdr = string.sub( shdr, 1, -2);
	local sbody = "";
	while( ODBC.Fetch(hsmt1) == 0 ) do
		local rows = "["
		for i = 1,cols1 do
			 rows = rows.."\""..ODBC.GetData(hsmt1,i).."\",";
		end
		rows = string.sub( rows, 1, -2);
		sbody = sbody..rows.."],";
	end
	sbody = string.sub( sbody, 1, -2);
	ODBC.freeHandle(hsmt1);
	local s = string.format("{\"head\":[%s],\"body\":[%s]}",shdr,sbody);
	return s;
end

function getdnslist()
	print("getdnslist");
	local sjson = "";	
	for k, v in pairs(mydns) do
		sjson = sjson .. "'"  .. k .. "',";
	end	
	sjson = string.sub( sjson, 1, -2);
	return string.format("[%s]",sjson);
end

-------------------------------------------------信息点表比较过虑数据存储操作-----------------------------------------------------------------------------------------------
function compareLoadgroups()
	print("lua:compareLoadgroups");
	local sj = "";
	local mt = SQLITE.prepare( mydb, string.format("select groupname from cmpfilters"));
	while SQLITE.setup( mt ) == 0 do
		sj = sj  .. "'" .. SQLITE.column_text(mt,0) .. "',";
	end
	SQLITE.finalize(mt);
	sj = string.sub( sj, 1, -2);
	return string.format("[%s]",sj);
end
function compareLoadfilters( group )
	print("lua:compareLoadfilters:" .. group);
	local sj = "";
	local mt = SQLITE.prepare( mydb, string.format("select words from cmpfilters where groupname='%s'",group));
	if SQLITE.setup( mt ) == 0 then
		local words = XAPI:split( SQLITE.column_text(mt,0) , ";");
		for k, v in pairs(words) do
			sj = sj .. "'"  .. v .. "',"
		end	
	end
	SQLITE.finalize(mt);
	return string.format("[%s]",string.sub( sj, 1, -2));
end
function compareSavefilters( group, data )
	print("compareSavefilters:" .. group .. " data:" .. data );
	local strsql = string.format("replace into cmpfilters( groupname, words ) VALUES('%s','%s')",group,data);
	local dbret = SQLITE.execute(mydb, strsql);
	return string.format("%d",dbret);
end
function compareRemovefilters( group )
	print("compareRemovefilters:" .. group  );
	local strsql = string.format("delete from cmpfilters where groupname='%s'",group);
	local dbret = SQLITE.execute(mydb, strsql);
	return string.format("{'ret':%d}",dbret);
end

function compareTaskCreate( stationid,tasktype,taskclass,taskname, jsontask )
	local str = string.gsub( jsontask,'|', ',');	
	local strsql= string.format("replace into cstasks(stationid,tasktype,taskclass,taskname,taskdesc) VALUES(%s,'%s','%s','%s','%s')",stationid,tasktype,taskclass,taskname,str);
	print( strsql );
	local dbret = SQLITE.execute(mydb, strsql);
	--local mt = SQLITE.prepare( mydb, "select last_insert_rowid()");
	--if SQLITE.setup( mt ) == 0 then
	--	local id = SQLITE.column_text(mt,0);
	--end
	--SQLITE.finalize(mt);
	return string.format("{'ret':%d}",dbret);
end 

function compareTaskQuery( stationid,tasktype,taskclass )
	print("compareTaskQuery:" ..  stationid .. " class:".. taskclass);
	local staname =  stationArray[tonumber(stationid)] ;
	local retjson = "{'tasks':[";
	local strsql = string.format("select autoid,taskname,taskdesc,splite from cstasks where (stationid=%d or stationid=-1)  and tasktype='%s' and taskclass='%s'",stationid,tasktype,taskclass);
	local mt = SQLITE.prepare( mydb, strsql);
	local empty = false;
	while SQLITE.setup( mt ) == 0 do
		local id = SQLITE.column_text(mt,0);
		local name = SQLITE.column_text(mt,1);
		local desc = SQLITE.column_text(mt,2);
		local spstr = SQLITE.column_text(mt,3);
		local hist  = showcmpstatichistory( staname, id );
		retjson = retjson .. string.format("{'id':%d,'name':'%s',%s,'splite':'%s','history':%s},",id,name,string.gsub( desc,'"', "'"),spstr,hist);
		empty = true;
	end
	SQLITE.finalize(mt);
	if empty == false then
		retjson =  retjson.."]}";
	else
		retjson = string.sub(retjson,1,-2) .."]}";
	end
	print( retjson );
	return retjson;
end

function showcmpstatichistory( stationname , taskid )
	print("showcmpstatichistory sn:"..stationname.."  taskid:"..taskid);
	local sj = ""
	local vd = SYS.ListDirFile(  string.format("./data/%s/static/%s/*",stationname,taskid) );
	if vd then
		for j = 0, #vd, 1 do
			local sf = ""
			local vf = SYS.ListDirFile( string.format("./data/%s/static/%d/%s/*",stationname,taskid,vd[j]) );
			if vf  then
				for k = 0, #vf, 1 do
					sf = sf .. "\'" .. vf[k] .."\',";
				end
				sf = string.sub(sf,1,-2);
				sj = sj .."{\'dir\':\'" .. vd[j] .. "\',\'files\':[" .. sf .. "]},";
			end
		end
		sj = string.sub( sj, 1, -2);
	end
	return string.format("[%s]",sj);
end

-------------------------------------------------------模块启动信息---------------------------------------------------------------------------------------------------
sermonProc = coroutine.create(function()
	sermonobj = require "sermon"
	sermonobj.start(100,function(tag,seq,func,dir,tms,data,len)
			realdataprocess( tag, data );
		end);
	for k, v in pairs(myModules) do
		sermonobj.createcnn(k,v.storage,v.chntype);
		sermonobj.setparams(k,v.params);
		sermonobj.setdelay(k, 20)
	end
end)

--websocket模块
websockproc = coroutine.create(function()
	websockobj = require "websock"
	websockobj.start("0.0.0.0",2980,function(act,evt,data)
		if evt > 0 then
			print( data );
			local cmd = JSON:decode( data );
			if cmd.action == "start" then
			elseif cmd.action == "stop" then
			end
		elseif evt == 0 then
		end
	end);
end)

mainProc = coroutine.create(function(t)install(t,
	function( addr, data, len )
		funcode = string.byte(string.sub(data,1,1));
		tagname = string.sub(data,2,len);
	end,
    function( sn )
	end,
	function()
		print("lua mainproc exit!");
	end)
end)

startup();
coroutine.resume(mainProc,1000);
coroutine.resume(sermonProc);
coroutine.resume(websockproc);
print("start infinalize");
