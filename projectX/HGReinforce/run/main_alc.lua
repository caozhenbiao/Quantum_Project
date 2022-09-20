print("start alc at:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local JSON = require "luajson"
local XAPI = require "luaxapi"
local SYS  = require "system"
local REGEDIT = require "regedit"
local QUERY = 0;
local REVERT = 1;
local EXECUTE = 2;
local HKEY_CLASSES_ROOT = 0x80000000;
local HKEY_CURRENT_USER  = 0x80000001;
local HKEY_LOCAL_MACHINE  = 0x80000002;
local HKEY_USERS  = 0x80000003;
print("load module success!")

--A1:删除共享
function A1_execute( data )	
	local detail = {};
	detail.unShare = {};
	for _,v in pairs(data.share) do
		local ret = SYS.cmdExecute(string.format("net share %s /delete",v.name));
		table.insert(detail.unShare,v);
	end
	local ret,hkey = REGEDIT.openKey(HKEY_CLASSES_ROOT,"System\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\");
	if ret == 0 and hkey > 0 then
		REGEDIT.writeIntValue( hkey,"AutoShareServer", 0 );
		REGEDIT.writeIntValue( hkey,"AutoShareWKS", 0 );
		REGEDIT.closeKey( hkey );
		detail.AutoShareServer = 0;
		detail.AutoShareWKS = 0;
	end
	return 0,detail;
end
function A1_revert( data )
	for _,v in pairs(data.share) do
		local echo = SYS.cmdExecute(string.format("net share %s=%s",v.name, v.path));
	end
	local ret,hkey = REGEDIT.openKey(HKEY_CLASSES_ROOT,"System\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\");
	if ret == 0 and hkey then
		REGEDIT.writeIntValue( hkey,"AutoShareServer", data.AutoShareServer );
		REGEDIT.writeIntValue( hkey,"AutoShareWKS", data.AutoShareWKS );
		REGEDIT.closeKey( hKey );
	end
	return ret;
end
function A1_query()
	local data = {};
	data.share = {};
	local retStr = SYS.cmdExecute("net share");
	local shares = retStr:gmatch("(%w+$)%s*(%w:\\+)");
	for k,v in shares do
		table.insert(data.share,{name=k,path=v});
	end
	local ret,hkey = REGEDIT.openKey(HKEY_CLASSES_ROOT,"System\\CurrentControlSet\\Services\\LanmanServer\\Parameters\\");
	if ret == 0  then
		local r1,v1 = REGEDIT.readIntValue(hkey, "AutoShareServer");
		local r2,v2 = REGEDIT.readIntValue(hkey, "AutoShareWKS");
		REGEDIT.closeKey( hkey );
		data.AutoShareServer = v1;
		data.AutoShareWKS = v2;
	end
	return data;
end


--A：密码策略设置符合复杂度要求
function A2_execute( data )
	print("A2 EXECUTE")
	local detail = {};
	local file = io.open("gp.cfg", "a");
	io.output(file);
	io.write("[version]\n");
	io.write("signature=\"$CHICAGO$\"\n");
	io.write("[System Access]\n");
	io.write("MinimumPasswordAge = 0\n");
	io.write("MaximumPasswordAge = 13\n");
	io.write("MinimumPasswordLength = 0\n");
	io.write("PasswordComplexity = 0\n");
	io.write("PasswordHistorySize = 0\n");
	io.close(file);
	local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
	local retsr = SYS.cmdExecute( cmdstr );
	detail.MinimumPasswordAge = 0;
	detail.MaximumPasswordAge = 13;
	detail.MinimumPasswordLength = 0;
	detail.PasswordComplexity = 0;
	detail.PasswordHistorySize = 0;
	return 0,detail;
end
function A2_revert( data )
	local file = io.open("gp.inf", "a");
	io.output(file);
	io.write("[version]\n");
	io.write("signature=\"$CHICAGO$\"\n");
	io.write("[System Access]\n");
	for k, v in pairs( data ) do
		print( k, v )
		io.write(string.format("%s = %d\n", k, v));
	end 
	io.close();
	local retStr = SYS.cmdExecute("secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.inf /areas SECURITYPOLICY");
	print( retStr );
	return 0;
end
function A2_query()
	local data = {};
	local command_rd = "secedit /export /cfg luan.inf > 0 & type luan.inf | findstr /R /i \"^min ^max ^pass ^lock\"";
	local retStr = SYS.cmdExecute( command_rd );
	print( retStr );
	for k, v in string.gmatch(retStr, "(%w+) = (%d+)") do
	
		print( k, v );
		data[ k ] = tonumber(v);
	end
	return data;
end

--启用账户锁定策略
function A3( opt )
	print("A3:ACCOUNT LOCK POLICY")
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		local file = io.open("gp.cfg", "w+");
		io.output(file);
		io.write("[System Access]\n");
		io.write("LockoutBadCount = 3\n");
		io.write("ResetLockoutCount = 5\n");
		io.write("LockoutDuration = 5\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
		local file = io.open("gp.cfg", "w+");
		io.write("[System Access]\n");
		io.write("LockoutBadCount = 0\n");
		io.write("ResetLockoutCount = 0\n");
		io.write("LockoutDuration = 0\n");
		io.close(file);
		local retStr = SYS.cmdExecute("secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY");
		context = "revert config success";
		print( retStr );
	else
		local command_rd = "secedit /export /cfg luan.inf > 0 & type luan.inf | findstr /R /i \"^LockoutBadCount ^ResetLockoutCount ^pass ^LockoutDuration\"";
		local retStr = SYS.cmdExecute( command_rd );
		context = string.gsub(retStr, " ", "")
	end
	return result, context;
end

--用户权限设置


--默认账户的权限控制
function A4( opt )
	print("A3:DEFAULT ACCOUNT POLICY CONTROL")
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[System Access]\n");
		io.write("EnableGuestAccount = 0\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
		local file = io.open("gp.inf", "a");
		io.output(file);
		io.write("[System Access]\n");
		io.write("EnableGuestAccount = 1\n");
		local retStr = SYS.cmdExecute("secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY");
		context = "revert config success";
		print( retStr );
	else
		local command_rd = "secedit /export /cfg luan.inf > 0 & type luan.inf | findstr /R /i \"^EnableGuestAccount\"";
		local retStr = SYS.cmdExecute( command_rd );
		context = string.gsub(retStr, " ", "")
	end
	return result, context;
end

--删除或禁用多余的、过期的帐户
function A5_execute( data )
	XAPI:PrintTable( data )
	
	
	
end
function A5_revert( data )

end
function A5_query()
	local data = {};
	local userList = SYS.ListUserInfo();
	for k, v in pairs(userList) do
		local item = JSON:decode( v );
		table.insert( data, item );
	end
	return data;
end



--应用软件的账户权限设置
function A6( opt )
	return 0, "A6";
end


function A7( opt )
	return 0, "A7";
end

function A8( opt )
	return 0, "A10";
end


--审核策略
function A9( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Event Audit]\n");
		io.write("AuditSystemEvents = 3\n");
		io.write("AuditLogonEvents = 3\n");
		io.write("AuditObjectAccess = 3\n");
		io.write("AuditPrivilegeUse = 3\n");
		io.write("AuditPolicyChange = 3\n");
		io.write("AuditAccountManage = 3\n");
		io.write("AuditProcessTracking = 3\n");
		io.write("AuditDSAccess = 3\n");
		io.write("AuditAccountLogon = 3\n");
		io.close(file);
		--local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		--local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Event Audit]\n");
		io.write("AuditSystemEvents = 0\n");
		io.write("AuditLogonEvents = 0\n");
		io.write("AuditObjectAccess = 0\n");
		io.write("AuditPrivilegeUse = 0\n");
		io.write("AuditPolicyChange = 0\n");
		io.write("AuditAccountManage = 0\n");
		io.write("AuditProcessTracking = 0\n");
		io.write("AuditDSAccess = 0\n");
		io.write("AuditAccountLogon = 0\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	else
		context = "A9";
	end
	return result, context;
end

--设置系统日志
function A10( opt )
	return 0, "A10";
end

--禁用不必要服务
function A11( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		--SYS.disableService("Alerter");
		--SYS.disableService("Clipbook");
		--SYS.disableService("Messager");
		--SYS.disableService("telnet");
		--SYS.disableService("smtp");
		--SYS.disableService("update");
	elseif opt == REVERT then
 
	else
		context = "A11";
	end
	return result, context;
end

--加固SNMP服务、配置SNMP TRAP
function A12( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
 
	elseif opt == REVERT then
 
	else
		context = "A12"
	end
	return result, context;
end

function A13( opt )
	return 0, "A10";
end

function A14( opt )
	return 0, "A10";
end


--禁用匿名远程连接
function A15( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
 		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\NullSessionShares=7,\n");
 		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\NullSessionPipes=7,\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
  		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\NullSessionShares=7,\n");
 		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\NullSessionPipes=7,ABC\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	else
		context = "Alertter";
	end
	return result, context;
end

--启用交互式登录(不显示上次的用户)
function A16( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
 		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
 		io.write("MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\DontDisplayLastUserName=4,0\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
  		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
		io.write("MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\DontDisplayLastUserName=4,1\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	else
		context = "Alertter";
	end
	return result, context;
end

function A17( opt )
	return 0, "A17";
end

function A18( opt )
	return 0, "A18";
end

--禁止用户开机自动登录
function A19( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		local bWrite = 0;
		local ret, hkey = REGEDIT.openKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\WindowsNT\\CurrentVersion\\Winlogon");	
		if ret== 0 and hkey then
			bWrite = REGEDIT.writeIntValue( hkey,"AutoAdminLogon", 0 );
			REGEDIT.closeKey( hKey );
		end
		context = string.format("{'result':0,'AutoAdminLogon':%d}",bWrite);
	elseif opt == REVERT then
 
	else
		local open = 0;
		local ret, hkey = REGEDIT.openKey(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\WindowsNT\\CurrentVersion\\Winlogon");	
		if ret== 0 and hkey then
			open = REGEDIT.ReadIntValue( hkey,"AutoAdminLogon", 0 );
			REGEDIT.closeKey( hKey );
		end
		context = string.format("{'AutoAdminLogon':%d}",open);
	end
	return result, context;
end

--检测 DDOS攻击保护设置
function A20( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
		local ret, hkey = REGEDIT.openKey(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters");	
		if ret== 0 and hkey then
			REGEDIT.writeIntValue( hkey,"SynAttackProtect", 2 );
			REGEDIT.writeIntValue( hkey,"TcpMaxPortsExhausted", 5 );
			REGEDIT.writeIntValue( hkey,"TcpMaxHalfOpen", 500 );
			REGEDIT.writeIntValue( hkey,"TcpMaxHalfOpenRetried", 400 );
			REGEDIT.writeIntValue( hkey,"EnableICMPRedirect", 0 );
			REGEDIT.closeKey( hKey );
		end
	elseif opt == REVERT then
	else
		context = "DDOS";
	end
	return result, context;
end

--检测网络服务挂起时间
function A21( opt )
	local result = 0;
	local context = "";
	if opt == EXECUTE then
 		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
 		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\AutoDisconnect=4,15\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	elseif opt == REVERT then
  		local file = io.open("gp.cfg", "a");
		io.output(file);
		io.write("[Registry Values]\n");
		io.write("MACHINE\\System\\CurrentControlSet\\Services\\LanManServer\\Parameters\\AutoDisconnect=4,20\n");
		io.close(file);
		local cmdstr = "secedit /configure /db %windir%\\security\\policy.sdb /cfg gp.cfg /areas SECURITYPOLICY";
		local retStr = SYS.cmdExecute( cmdstr );
		context = "set config success";
		print( "exec secedit finish");
	else
		context = "Alertter";
	end
	return result, context;
end


--配置登录终端的操作超时锁定

function A22( opt )
	return 0, "A22";
end

function A23( opt )
	return 0, "A23";
end

function A24( opt )
	return 0, "A24";
end

function A25( opt )
	return 0, "A25";
end

function A26( opt )
	return 0, "A26";
end

function A27( opt )
	return 0, "A27";
end


local TaskA = {}

function TaskA:execute( code, data )
	local ret = 0;
	local ctx = "";	
	if code=="A1"     then ret,ctx=A1_execute( data );
	elseif code=="A2" then ret,ctx=A2_execute( data );
	end
	return ret,ctx;
end

function TaskA:revert( code, data )
	local ret = 0;
	if code=="A1" then ret=A1_revert( data );
	elseif code=="A2" then ret=A2_revert( data );
	end
	return ret;
end

function TaskA:query( code )
	local data;
	if code == "A1" then data = A1_query();
	elseif code == "A2" then data = A2_query();
	elseif code == "A5" then data = A5_query();
	end
	return data;
end

function TaskA:new(args)
   local new = { }
   if args then
      for key, val in pairs(args) do
         new[key] = val
      end
   end
   return setmetatable(new, TaskA)
end

TaskA.__index = TaskA
return TaskA:new()