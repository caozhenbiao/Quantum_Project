 
local REGEDIT = require "regedit"
 
--删除共享
function B1()
	print("A1:删除共享")
 	local retStr1 = REGEDIT.cmdPopen("sc config 服务名 start= disabled");
	
end

--设置密码长度
function B2()
	print("A2:禁止用户开机自动登录")
	--HKEY_LOCAL_MACHINE\Software\Microsoft\WindowsNT\CurrentVersion\Winlogon，新建一个名称为AutoAdminLogon，类型为REG_DWORD，值为0的键值。如果存在AutoAdminLogon 则直接将键值改为0。
	 
end

--关闭Windows自动播放功能
function B3()
	print("B3:关闭Windows自动播放功能")
	
end

--禁用可远程访问的注册表路径和子路径
function B4()
	print("B4:禁用可远程访问的注册表路径和子路径")
	
end


--删除多余的、过期的帐户
function B5()
	print("B6:删除多余的、过期的帐户")
	
end

--默认账户的权限控制
function B7()
	print("B4:默认账户的权限控制")
	
end


--禁用匿名远程连接
function B8()
	print("B4:禁用匿名远程连接")
	
end


