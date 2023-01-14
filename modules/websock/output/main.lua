--*****LUA SERMON SCRIPT*****--
print("this is websock test script. start at " .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local websockobj;
 
--websocket模块
websockproc = coroutine.create(function()
	websockobj = require "websock"
	websockobj.start("0.0.0.0",2980,function(evt,len,data)
		print( data );
		return 1;
	end);
end)
 
--LUA脚本宿主,安装事件处理函数
mainProc = coroutine.create(function(t)install(t,
    function( sn )
		print("timer:" .. sn .. "\n");
	end,
	function()
		print("close...\n");
		print("lua mainproc exit!");
	end)
end)

coroutine.resume(websockproc);
coroutine.resume(mainProc,1000);
print("start infinalize")
