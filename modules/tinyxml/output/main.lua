--*****LUA SQLITE SCRIPT*****--
print("this is tinyxml test script. start at " .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local tinyxmlobj = require "tinyxml"
print("load tinyxml finish");
 
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

coroutine.resume(mainProc,1000);
print("start infinalize")
