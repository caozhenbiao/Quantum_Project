--**LUA SERMON SCRIPT*****--
print("smartx project issue script start:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()));
print("lua  make.lua script version 2020-04-23")

local vaildday  = -10;
local expdesc = "你的程序试用期已过";
local procname = "HGPDA"
local deplaydir = "../../deploy"

local function urlEncode(s)
     s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
    return string.gsub(s, " ", "+")
end 

local function urlDecode(s)
    s = string.gsub(s, '%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end)
    return s
end

function escape(s)
    return string.gsub(s, "([^A-Za-z0-9_])", function(c) 
	if string.byte(c) > 127 then
		return c;
	end
	return string.format("%%%02x", string.byte(c)) 
	end)
end

function unescape(s)
    return string.gsub(s, "%%(%x%x)", function(hex) return string.char(tonumber(hex, 16)) end)
end

function strippath(filename)  
    return string.match(filename, ".+/([^/]*%.%w+)$") -- *nix system  
    --return string.match(filename, “.+\\([^\\]*%.%w+)$”) — *nix system  
end  

--WINDOW & LINUX 文件夹分割符
function formatdir( path )
	if os.getenv("OS") == "Windows_NT" then
		return string.gsub(path,'/','\\')
	end
	return path;
end

--拷贝文件到某一目录
function copyfile( src, path )
	if os.getenv("OS") == "Windows_NT" then
		src = string.gsub(src,"%.so",".dll")
		print( string.format("copy file %s %s", src, path) )
		local cmd = string.format("copy %s %s", formatdir(src), formatdir(path));
		os.execute( cmd );
	else
		print( string.format("copy file %s %s", src, path) )
		local cmd = string.format("cp -f %s %s", src, path);
		os.execute( cmd );
	end
end

function authcopy( src, path )
	print( src .. " path: ".. path);
	local file = io.open(src, "rb");
	local strs = file:read("*all");
	file:close(file);
	local bom = string.sub(strs,1,3)
	local fstrs  = string.sub(strs,4,string.len(strs)) 
	local xfstr   = string.format("<script>var n=parseInt((new Date()).valueOf()/86400000); if(n>%d)alert(\"%s\");</script>", math.floor(os.time()/86400)+vaildday,expdesc);
	local filew;
	if os.getenv("OS") == "Windows_NT" then
		filew = assert(io.open(path .."\\" ..  string.match(src, ".+\\([^\\]*%.%w+)$"),'wb'));
	else
		filew = assert(io.open(path .."/" ..  string.match(src, ".+/([^/]*%.%w+)$"),'wb'));
	end
	filew:write(bom);
	filew:write( xfstr );
	filew:write( fstrs );
	filew:close();
end

--拷贝文件夹下所有文件及文件夹（递归）
function copydir( src, dest )
	print( string.format("copy dir %s %s", src, dest))
	if os.getenv("OS") == "Windows_NT" then
		local cmd = string.format("XCOPY %s %s\\ /E /S /Y", formatdir(src), formatdir(dest));
		os.execute( cmd )
	else
		local cmd = string.format("cp -rf %s %s", src, dest);
		os.execute( cmd );
	end
end

--拷贝目录下所有文件
function copydirfiles( src, path )
	print( string.format("copy dir files at %s", path ))
	if os.getenv("OS") == "Windows_NT" then	
		local dir = string.match( formatdir(src), ".+\\([^\\]*%w+)$");
		local cmd = string.format("XCOPY %s\\* %s\\%s\\* /E /S /Y", src, path, dir);
		os.execute( cmd );
	else
		local dir = string.match(src, ".+/([^/]*%w+)$");
		local cmd = string.format("cp -rf %s %s/%s", src, path, dir);
		print( cmd )
		os.execute( cmd );
	end
end

--crate directory
function createdir( path )
	print( string.format("create directory %s",path))
	os.execute("mkdir "..formatdir(path));
end

--reove directory
function removedir( path )
	if os.getenv("OS") == "Windows_NT" then
		os.execute("rd /s /q "..formatdir(path))
	else	
		os.execute("rm -rf "..path)
	end
end

--压缩文件夹
function zipdirectory( path )
	if os.getenv("OS") == "Windows_NT" then	
		local outpkg = string.format("%s/%s_win_%s.zip",path,procname,os.date("%Y%m%d%H%M", os.time()))
		print ( outppkg )
		local cmd = string.format("..\\..\\tools\\7za.exe a %s %s \\*",outpkg,path,procname);
		print( cmd )
		os.execute( cmd )

		--os.execute("..\\..\\tools\\7za.exe a .\\output\\" .. outpkg ..  " .\\output\\" .. procname .. "\\*" );
		--local outpkg = string.format("%s%s.zip",procname,os.date("%Y%m%d%H%M", os.time()))
		--os.execute("..\\..\\tools\\7za.exe a .\\output\\" .. outpkg ..  " .\\output\\" .. procname .. "\\*" );
	else
		
		local outpkg = string.format("%s_linux_%s.tar.gz",procname,os.date("%Y%m%d%H%M", os.time()))
		os.execute("cd "..deplaydir.." && tar -cf "..outpkg.." "..procname);
	end
end

--创建启动脚本
function makeauthbash( runpath )
	if os.getenv("OS") == "Windows_NT" then
		local filew = assert(io.open(runpath.."/smartx_run.bat",'wb'));
		filew:write( "cd run\n");
		filew:write( "smartx.exe -s ./main.lua -y 8008 -k 7C121F1E009326EF67B3426E7D72162DA01B0F82\n");
		filew:close();
	else
		local filew = assert(io.open(runpath.."/smartx_run.sh",'wb'));
		filew:write( "#! /bin/sh\n");
		filew:write( "prp = $(ps -ef | grep \"smartx.exe\" | grep -v grep | grep -v \"$0\")\n" );
		filew:write( "if [ -z \"$pro\" ]\n" );
		filew:write( "then\n");
		filew:write( "	cd ./run/\n");
		filew:write( "	./smartx.exe -s ./main.lua -y 8006 -k 7C121F1E009326EF67B3426E7D72162DA01B0F82\n");
		filew:write( "else\n");
		filew:write( "	echo \"smartx is runing.\"\n");
		filew:write( "fi");
		filew:close();
		os.execute(string.format("chmod a+x %s/%s", runpath, "smartx_run.sh"))
	end
end

function makebrowerconfig( path )
	local filew = assert(io.open(path.."/config.ini",'wb'));
	filew:write("[app]\n");
	filew:write("title = 信息点表比对\n");
	filew:write("key = 7C121F1E009326EF67B3426E7D72162DA01B0F82\n");
	filew:write("main = mainx.lua\n");
	filew:write("home = http://localhost:8008/index.html\n");
	filew:write("fullscreen = 0\n")
	filew:write("width = 1280\n");
	filew:write("height = 900\n");
	filew:close();
end

----------------------------------------------------------------------------------------------------------------------------------------------------------
function dopackage()
	local destpath = deplaydir .. "/" .. procname;
	local manpath  = destpath .. "/man";
	local runpath  = destpath .. "/run";
	local clipath  = destpath .. "./browser";

	removedir( destpath );
	createdir( destpath );
	createdir( manpath );
	createdir( runpath );
	createdir( clipath );
	makebrowerconfig( clipath );
	makeauthbash(destpath);

	copyfile("./README", destpath);
	copydir("./run/protocol", runpath );
	copyfile("./run/luajson.lua",  runpath);
	copyfile("./run/luabit.lua",  runpath);
	copyfile("./run/luamd5.lua",  runpath);
	copyfile("./run/luaxapi.lua",  runpath);

	if os.getenv("OS") == "Windows_NT" then
		copyfile("./run/smartx.exe",  runpath);
		copyfile("./run/luadll.dll",  runpath);
		copyfile("./run/pthread.dll",  runpath);
		copyfile("./run/sermon.dll",  runpath);
		copyfile("./run/sqlite.dll",  runpath);
		copyfile("./run/system.dll",  runpath);
		copyfile("./run/websock.dll",  runpath);
		copyfile("../../platform/webui/bin/webui.exe",runpath);
		copydir("../../platform/webui/QtLib/x86",clipath);
		copyfile("../../platform/smartx/libs/luadll.dll",  clipath);
		copyfile("../../platform/smartx/libs/pthread.dll",  clipath);

	else
		copyfile("../../platform/smartx/bin/smartx_linux.exe",runpath.."/smartx.exe");
		copyfile("./run/sermon.so",  runpath);
		copyfile("./run/sqlite.so",  runpath);
		copyfile("./run/system.so",  runpath);
		copyfile("./run/websock.so",  runpath);
		os.execute(string.format("chmod a+x %s/%s", runpath, "smartx.exe"))
		copyfile("../../platform/webui/bin/webui.exe",runpath);
		os.execute(string.format("chmod a+x %s/%s", runpath, "webui"));
		copyfile("../../platform/smartx/libs/luadll.so",  clipath);
		copyfile("../../platform/smartx/libs/pthread.so",  clipath);	
		copyfile("../../platform/smartx/libs/luadll.so",  clipath);
		copyfile("../../platform/smartx/libs/pthread.so",  clipath);
	end

	--web
	local webpath =  runpath .. "/webui";
	createdir( webpath );
	copydir("./run/webui/css", webpath );
	copydir("./run/webui/images", webpath );
	copydir("./run/webui/js", webpath );
	copyfile("./run/webui/about.html",webpath);
	copyfile("./run/webui/botechk.html",webpath);
	copyfile("./run/webui/chatview.html",webpath);
	copyfile("./run/webui/debugview.html",webpath);
	copyfile("./run/webui/device.html",webpath);
	copyfile("./run/webui/devopts.html",webpath);
	copyfile("./run/webui/historydata.html",webpath);
	copyfile("./run/webui/index.html",webpath);
	--authcopy("./run/webui/mainfrm.html",webpath);
	copyfile("./run/webui/options.html",webpath);
	copyfile("./run/webui/pointmgr.html",webpath);
	copyfile("./run/webui/realtimedata.html",webpath);
	copyfile("./run/webui/usermgr.html",webpath);

	zipdirectory( destpath )
	--removedir( destpath );
	print("package finish")
end

function rebuild()
	os.execute("cd ../../platform/smartx/libs/lua && make clean && make linux")	
	os.execute("cd ../../platform/smartx/src && make clean && make");		
	os.execute("cd ../../modules/websock && make clean && make")
	os.execute("cd ../../modules/sermon/libcap/linux && make clean && ./configure && make")
	os.execute("cd ../../modules/sermon/libctb/build && make clean && make")
	os.execute("cd ../../modules/sermon && make clean && make")
	os.execute("cd ../../modules/sqlite && make clean && make")
	os.execute("cd ../../modules/system && make clean && make")
	os.execute("cd ../../modules/unicom && make clean && make")
	os.execute("cd ../../modules/tinyxml && make clean && make")
	print("build finish")
end

function update()
	local runpath  =  "./run";
	local clipath  =  "./browser";
	if os.getenv("OS") == "Windows_NT" then
		copyfile("../../platform/smartx/bin/smartx.exe",  runpath);
		copyfile("../../platform/smartx/libs/pthread.dll",  runpath);
		copyfile("../../platform/smartx/libs/pthreadD.dll",  runpath);
		copyfile("../../platform/smartx/libs/luadll.dll",  runpath);
		copyfile("../../platform/smartx/libs/luadllD.dll",  runpath);
		copyfile("../../platform/webui/bin/webui.exe",clipath);
		copydir("../../platform/webui/QtLib/x86",clipath);
		copyfile("../../platform/smartx/libs/luadll.dll",  clipath);
		copyfile("../../platform/smartx/libs/pthread.dll",  clipath);
	else
		copyfile("../../platform/smartx/bin/smartx_linux.exe","./run/smartx");
		copyfile("../../platform/webui/bin/webui",clipath);
		os.execute(string.format("chmod a+x %s/%s", clipath, "webui"));	
		copyfile("../../platform/smartx/libs/luadll.so",  clipath);
		copyfile("../../platform/smartx/libs/pthread.so",  clipath);
	end

	makebrowerconfig( clipath );
	copyfile("../../modules/websock/bin/websock.so",  runpath)
	copyfile("../../modules/sermon/bin/sermon.so",  runpath)
	copyfile("../../modules/sqlite/bin/sqlite.so",  runpath)
	copyfile("../../modules/system/bin/system.so",  runpath)
	copyfile("../../modules/odbc/bin/odbc.so",  runpath);
	copyfile("../../modules/unicom/bin/unicom.so",  runpath)
	copyfile("../../modules/sermon/bin/sermon.so",  runpath)
	copyfile("../../modules/system/bin/system.so",  runpath)
	copyfile("../../modules/tinyxml/bin/tinyxml.so",  runpath)
	print("update finish")
end

function licence()
	local SYS   = require "system"
	local MD5   = require "luamd5"
	local a, b ,c , d = SYS.cpuinfo();
	local prikey = string.format("%s%10d",c,b);
	local md5    = MD5.tohex(MD5.sum(prikey));
	print( "lua:" .. md5 );
	print( "sum:" .. SYS.authorize(prikey) );
end

--do proc	
for i, v in ipairs(arg) do
	if os.getenv("OS") ~= "Windows_NT" then
		if v == "build"   then rebuild() end
	end
 	if v == "update"  then update() end
	if v == "package" then dopackage() end
end

os.exit();





