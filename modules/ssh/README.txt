
SSH
 
 SSH INTERFACE:
	1. int connect(ip,port,name,password);
	2. int connect_privatekey(ip,port,name,keyfile);
	3. int request(handle,callback);
	    * function callback(responseText)
	4. int disconnect();



LUA EXAMPLE:

local SSH  = require "ssh"
handle1 = SSH.connect_privatekey("5.183.101.155", 22, "root","/mnt/hgfs/Quantum_Project_v1.0/projectX/TEST/run/node_id_rsa");
SSH.request(handle1, "vnstat -d\n", function(resonseText)
	print(resonseText);
	SSH.disconnect( handle1 )
end)