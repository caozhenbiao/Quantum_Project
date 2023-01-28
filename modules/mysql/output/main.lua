--*****LUA MYSQL SCRIPT*****--
print("this is mysql test script. start at " .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local mysqlobj = require "mysql"
print("load mysql finish");
 
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
