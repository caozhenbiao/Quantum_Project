WEBSOKET MODULE USAGE

INTERFACE:
	1. int start(ip, port, callback);
		function callback(act,len,data);
		if the function return not 0,websock will be disconnect.
	2. int stop();
	3. int sendfrm( size,buf );
	4. int sendutf8( str );
	5. int send( str );
	6. string getip();
	
LUA SERVICE EXAMPLE:
	local websockobj = require "websock"
	websockobj.start("127.0.0.1",2980,function(act,len,data)
		if len > 0 then
			print( data );
		end
	end);
	...
	websockobj.sendstr( "you are welcome!" );
	...
	websockobj.destory();
	
	
JS CLIIENT EXAMPLE:
	var websocket =null;
	websocket = new WebSocket("ws://127.0.0.1:2980"); 
	websocket.onopen = function (evt) { alert("ws open"); };  
	websocket.onclose = function (evt) { alert("ws close"); };  
	websocket.onerror = function (evt) { alert("ws error"); };          
	websocket.onmessage = function (evt) { 
		if( typeof(evt.data) == "string" ){
			alert( evt.data )
		}
	...
	if (websocket.readyState == websocket.OPEN) {
		websocket.send( "{\"action\":\"start\",\"target\":\"dsfsdfsdfsdfsdf\"}");  
	} 
	...
	websocket.close();