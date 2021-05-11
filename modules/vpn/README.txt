 vpn   模块只能在windows系统下使用。
 
 VPN INTERFACE:
	1. int installcert( cert_str );
	2. int uninstallcert();
	3. int connect(ip,user,pwd,callback);
	       function callback(reason,error)
	4. int disconnect();


LUA EXAMPLE:
--[[test]]--
local VPN  = require "vpn"
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

VPN.installcert( strCertInput );
local handle = VPN.connect("142.93.152.42","user1","jnpljyqoo8x142w5",function(reason,error)
		print("vpn nsg:" .. reason .. "reason:" .. error );
		end)
...
disconnect( handle )




