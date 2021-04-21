--**LUA SERMON SCRIPT*****--
print("smartx project issue script start:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()));
print("lua make.lua [arg]")

local vaildday = -10;
local expdesc = "你的程序试用期已过";
local procname = "webui"
local deplaydir = "/media/psf/Home/mystation/deploy"

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


function decopy1( src, path )
	print( src .. " path: ".. path);
	local file = io.open(src, "rb");
	local strs = file:read("*all");
	file:close(file);
	local fstrs  = string.sub(strs,1,string.len(strs)) 
	local filew = assert(io.open(path .."\\" ..  string.match(src, ".+\\([^\\]*%.%w+)$"),'wb'));
	filew:write( fstrs );
	filew:close();
end

function decopy( src, path )
	print( src .. " path: ".. path);
	local file = io.open(src, "rb");
	local strs = file:read("*all");
	file:close(file);
	local bom = string.sub(strs,1,3)
	local fstrs  = string.sub(strs,4,string.len(strs)) 
	local xfstr   = string.format("<script>var n=parseInt((new Date()).valueOf()/86400000); if(n>%d)alert(\"%s\");</script>", math.floor(os.time()/86400)+vaildday,expdesc);
	local escstr = escape(xfstr); 
	local escscr = escape(fstrs);
	local filew = assert(io.open(path .."\\" ..  string.match(src, ".+\\([^\\]*%.%w+)$"),'wb'));
	filew:write(bom);
	filew:write("<script>document.write(unescape('");
	filew:write( escscr );
	filew:write( escstr );
	filew:write("'))</script>");
	filew:close();
end

function vacopy( src, path )
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

function xxcopy( src, path )
	if os.getenv("OS") == "Windows_NT" then
		local dir = string.match(src, ".+\\([^\\]*%w+)$");
		local cmd = string.format("XCOPY %s\\* %s\\%s\\* /E /S /Y", src, path, dir);
		os.execute( cmd );
	else
		local dir = string.match(src, ".+/([^/]*%w+)$");
		local cmd = string.format("cp -rf %s %s/%s", src, path, dir);
		print( cmd )
		os.execute( cmd );
	end
end

function mycopy( src, path )
	if os.getenv("OS") == "Windows_NT" then
		local cmd = string.format("copy %s %s", src, path);
		os.execute( cmd );
	else
		local cmd = string.format("cp %s %s", src, path);
		os.execute( cmd );
	end
end

function dopackage()
	local destpath = deplaydir .. "/" .. procname;
	os.execute("rm -rf "..destpath)
	os.execute("mkdir " .. destpath);
	os.execute("mkdir " .. manpath);
	os.execute("mkdir " .. runpath);

	mycopy("./smartx_run.bat", destpath);
	mycopy("./readme.txt", destpath);

	xxcopy("./run/protocal", runpath );
	mycopy("./run/main.lua", runpath);
	mycopy("./run/luabit.lua",  runpath);
	mycopy("./run/luamd5.lua",  runpath);
	mycopy("./run/luaxapi.lua" ,  runpath);

	mycopy("./run/smartx.exe",  runpath);
	mycopy("./run/luadll.dll",  runpath);
	mycopy("./run/pthread.dll",  runpath);
	mycopy("./run/sermon.dll",  runpath);
	mycopy("./run/sqlite.dll",  runpath);
	mycopy("./run/system.dll",  runpath);
	mycopy("./run/websock.dll",  runpath);
	
	--web
	local webpath =  runpath .. "/webui";
	os.execute("mkdir " .. webpath);
	xxcopy("./run/webui/css", webpath );
	xxcopy("./run/webui/images", webpath );
	xxcopy("./run/webui/js", webpath );

	mycopy("./run/webui/about.html",webpath);
	mycopy("./run/webui/botechk.html",webpath);
	mycopy("./run/webui/chatview.html",webpath);
	mycopy("./run/webui/debugview.html",webpath);
	mycopy("./run/webui/device.html",webpath);
	mycopy("./run/webui/devopts.html",webpath);
	mycopy("./run/webui/historydata.html",webpath);
	vacopy("./run/webui/indexhg.html",webpath);
	mycopy("./run/webui/index.html",webpath);
	vacopy("./run/webui/mainfrm.html",webpath);
	mycopy("./run/webui/options.html",webpath);
	mycopy("./run/webui/pointmgr.html",webpath);
	mycopy("./run/webui/realtimedata.html",webpath);
	mycopy("./run/webui/usermgr.html",webpath);

	if os.getenv("OS") == "Windows_NT" then
		local outpkg = string.format("%s%s.zip",procname,os.date("%Y%m%d%H%M", os.time()))
		os.execute("..\\..\\tools\\7za.exe a .\\output\\" .. outpkg ..  " .\\output\\" .. procname .. "\\*" );
	else
		
		local outpkg = string.format("%s%s.tar.gz",procname,os.date("%Y%m%d%H%M", os.time()))
		os.execute("cd "..deplaydir.." && tar -cf "..outpkg.." "..procname);
	end
	os.execute("rm -rf "..destpath)
	print("package finish")
end


function rebuild()
	os.execute("cd ../../smartx/src && make clean && make");
	os.execute("cd ../../smartx/libs/lualib && make clean && make")				
	os.execute("cd ../../modules/websock && make clean && make")
	os.execute("cd ../../modules/sermon && make clean && make")
	os.execute("cd ../../modules/sqlite && make clean && make")
	os.execute("cd ../../modules/system && make clean && make")
	print("build finish")
end

function update()
	if os.getenv("OS") == "Windows_NT" then
		os.execute("copy ..\\..\\smartx\\bin\\smartx.exe .\\run\\");
		os.execute("copy ..\\..\\smartx\\libs\\pthread.dll .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\pthreadD.dll .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\luadll.dll .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\luadllD.dll .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\lualib\\luajson.lua .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\lualib\\luaxapi.lua .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\lualib\\luamd5.lua .\\run\\")
		os.execute("copy ..\\..\\smartx\\libs\\lualib\\luabit.lua .\\run\\")
		os.execute("copy ..\\..\\modules\\websock\\bin\\websock.dll .\\run\\")
		os.execute("copy ..\\..\\modules\\sermon\\bin\\sermon.dll .\\run\\")
		os.execute("copy ..\\..\\modules\\sqlite\\bin\\sqlite.dll .\\run\\")
		os.execute("copy ..\\..\\modules\\system\\bin\\system.dll .\\run\\")
	else
		os.execute("cp ../../smartx/bin/smartx_linux.exe ./run/smartx.exe");
		os.execute("cp ../../smartx/libs/lualib/luajson.lua ./run/")
		os.execute("cp ../../smartx/libs/lualib/luaxapi.lua ./run/")
		os.execute("cp ../../smartx/libs/lualib/luamd5.lua ./run/")
		os.execute("cp ../../smartx/libs/lualib/luabit.lua ./run/")
		os.execute("cp ../../modules/websock/bin/websock.so ./run/")
		os.execute("cp ../../modules/sermon/bin/sermon.so ./run/")
		os.execute("cp ../../modules/sqlite/bin/sqlite.so ./run/")
		os.execute("cp ../../modules/system/bin/system.so ./run/")
	end
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
	if v == "build"   then rebuild() end
 	if v == "update"  then update() end
	if v == "package" then dopackage() end
end

os.exit();





