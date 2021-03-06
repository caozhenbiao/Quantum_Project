--**LUA SERMON SCRIPT*****--
print("load sermon project at:" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local JSON  = require "luajson"
--local NEWPROJECT   = require "new_project"
--local SQLITE  = require "sqlite"
local SSH  = require "ssh"
--local VPN  = require "vpn"
print("open library success!")


function table2json(t)
        local function serialize(tbl)
                local tmp = {}
                for k, v in pairs(tbl) do
                        local k_type = type(k)
                        local v_type = type(v)
                        local key = (k_type == "string" and "\"" .. k .. "\":")
                            or (k_type == "number" and "")
                        local value = (v_type == "table" and serialize(v))
                            or (v_type == "boolean" and tostring(v))
                            or (v_type == "string" and "\"" .. v .. "\"")
                            or (v_type == "number" and v)
                        tmp[#tmp + 1] = key and value and tostring(key) .. tostring(value) or nil
                end
                if #tbl == 0 then
                        return "{" .. table.concat(tmp, ",") .. "}"
                else
                        return "[" .. table.concat(tmp, ",") .. "]"
                end
        end
        assert(type(t) == "table")
        return serialize(t)
end

function web2lua( data )
	print("lua::web2lua")
	retval = {
		account = "caozhenbiao",
		password = "1231231231"
	}
	return table2json(retval)
end

function userlogin( data )
	retval = {
		account = "caozhenbiao",
		password = "1231231231"
	}
	return table2json(retval)
end

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
end

--LUA????????,????????????????
mainProc = coroutine.create(function(t)install(t,
	function( addr, data, len )
		print("trans...");
		funcode = string.byte(string.sub(data,1,1));
		tagname = string.sub(data,2,len);
	end,
    	function( sn )
		print("timer:" .. sn .."");
	end,
	function()
		NEWPROJECT.destory();
		print("lua mainproc exit!");
	end)
end)

function system_test( n )
	NEWPROJECT.request_delay( "this is a test",n,function(resonseText)
	
		print("newproject.callback_test:" .. resonseText);
		
		system_test( n )
	end)
end

function system_request()
	NEWPROJECT.request( "this is a test",function(resonseText)
		print("newproject.callback_test:" .. resonseText);
	end)
end


function format_vnstat( text )
	retval = {
		account = "caozhenbiao",
		password = "1231231231"
	}
	return table2json(retval)
end


function ssh_test()
	print("ssh_test start")
	--handle = SSH.connect( "192.241.252.154", 22, "root","JIEJHID9@didhi");
	--SSH.request(handle, "vnstat -d\n", function(resonseText)
	--	print(resonseText);
	--end)

	handle1 = SSH.connect_privatekey("5.183.101.155", 22, "root","/mnt/hgfs/Quantum_Project_v1.0/projectX/TEST/run/node_id_rsa");
	SSH.request(handle1, "vnstat -d\n", function(resonseText)
		print(resonseText);
		SSH.disconnect( handle1 )
	end)
end

--IPSEC VPN connect test
local strCertInput = [[-----BEGIN CERTIFICATE-----
MIIFPDCCAySgAwIBAgIIDOvX6TqzGHwwDQYJKoZIhvcNAQEMBQAwPDELMAkGA1UE
BhMCVVMxEzARBgNVBAoTClN0cm9uZ1N3YW4xGDAWBgNVBAMTD1RoaXNJc1NwYXJ0
YS53ZTAeFw0yMDAzMDQwNjM1NTVaFw0yMzAzMDQwNjM1NTVaMDwxCzAJBgNVBAYT
AlVTMRMwEQYDVQQKEwpTdHJvbmdTd2FuMRgwFgYDVQQDEw9UaGlzSXNTcGFydGEu
d2UwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQDMHgI5j5Y020yN8mXc
4vi / EvOPr3gV49VUdV2FH5qNsm5MYS3 + gBPyeaFCXIknyM5P1GxljcqozlQ3iDdM
snlHlOnQ5611aW / SZ4CQ7OvzNI5gKUWl6ffS1AyfSgbuslwt3VQQtzPbPsbs0Jqh
rR0ZDy0uUPgnLp80JpI6aAVwJL7nb1FyWON0oz2pFDk5epotQaBAsnQIajJZh2wv
gD + pG2g9gR8Wi9Tiu / 7SODhD + vsmBPnUUwpp7 / W1Llk043pXW / pkE61X7MJ6YU8M
rSiawBTr0eNo2kwCJTd2N / C8uDNpemZwFUL2uRJ8r5 / AcURQlnc3833WYAXoPpro
FT6 / 60jklQVMeYKEUTGG1Z7OrJXx0J2PEPYWah0 / KNgxD7ERsvGOFJ / p6cyUHxEB
UNVPfwZqJFEwyh / MkDlticjdUvgJLzbkWZCyvQuauBW / +oFnoRDylzmAXIDIl11B
yDPj5D / 5XBWYac3GtKYySirRR1RbTZaM2PzzXd07SY4eAJMAsrbFLIQ1gKeLu9ba
qRYcuUm0csQCoek06Q3Aj6MQkVAYmAnZr1 / m8XCBUEs9TMSdj77NifzPKbOMG5GJ
OWjgNGXPlTeSK6sRNeZw1ZOT2 / B9vErK3L70DtS / 0 + 4MMxRCFnU + BsLoKdJ2jccL
rFkAPyFrK26aIOWey9W2j3b4WQIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH / MA4G
A1UdDwEB / wQEAwIBBjAdBgNVHQ4EFgQUQKlLLTlblRz2AKzJfMKctKh + 0lQwDQYJ
KoZIhvcNAQEMBQADggIBAFlg0CWpug5uGBwywBZE5oz + rjxAVgB1uAmSpdLyVIzY
fE2Ag5x7HLTP2UXprTbdMD7 / KufBXNKKmoS8c6oV8QtArRC8RnxipNOSuV8cFBbt
j2TsYcwwnm513gB4AMXyuzGE / 1ByXubTzoXiKjtjIWOrkp + kG6m32QrvgyjZlVk9
y1cv3DiC5PkmluAzMUNW3SPYY8jQ3tUjvwLIC36LzQpAQ3sgesBw5pBWFz7 / DWZI
uUhLWV6wnFWWYmGPeUbT8UKj405uP8NKvDT3wof7taP6AVeQnp3bMNmsNMvYelBp
iIlnW8PetarU8qE6aivq6PyTwXPiDzYpkKYUcc4KIFnnwFEw0L0k4dl4zHld / zQh
KlowxhxhIkaK6t68drRa3iF4nxUJTUixClLwz0oIz / BsKNTKSq / E / 9m1HnUiotk0
RCxUwnMc + 9nGsmGOtIKGvLuTkMFHnS4Pds4Sakq0wmfm9IkLR2FpcTTNOmOB6yGS
VMe9 / dZbqdUDxISI7X6lr4bZ7FLM + 5 + bP4YOFq71avsO7kFAPPNlz3jaOEhl + cst
cZei4tum8be0KUFATIhQaGKhxqPuHd + KKL5xdycGRoeZGZBlziI6A1Is3oZ7mzCK
mrnn19f / ODAos0KfvWlS + QvkxdjcTh73pXs6cDfYMBr74INkl + Is / 2HvZvwbBJUX
-----END CERTIFICATE-----]];
--VPN.installcert( strCertInput );
--local hand = VPN.connect("142.93.152.42","user1","jnpljyqoo8x142w5",function(reason,error)
--		print("vpn nsg:" .. reason .. "reason:" .. error );
--		end)

--coroutine.resume(mainProc,10000);
ssh_test( );

		

 
