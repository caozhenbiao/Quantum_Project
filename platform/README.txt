quark : Kernel execute
webui : Qt webkit UI
directui: Direct UI


UASGE
k: auth key
s: lua script
u: user options
w: web root path
n: comm server port
x: web server ip
y: web server port
c: console module


javascript call lua interface with http request.
URL: file.function
ARG: json string


js:
//GetXmlHttpObject
function GetXmlHttpObject() {   
	var xmlHttp=null;   
	try{xmlHttp=new XMLHttpRequest();}
	catch(e){ 
		try{xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");}
		catch(e){xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");}     
	}
	return xmlHttp; 
}

//dispath lua interface test
function request(){
	var xhr = GetXmlHttpObject();     
	xhr.open("LUAPOST","main.request",true);     //LUAPOST:mathed; main:lua file; request:lua interface;
	xhr.setRequestHeader('Content-Type', 'application/json');	
	xhr.onreadystatechange = function(){
		if(xhr.readyState==4 && xhr.status==200){
			var myArr = JSON.parse(xhr.responseText);
			//alert( myArr.account );
		}
	}
	const json = {
		"cmd": "dir",
		"password": "cityslicka"
	};
	xhr.send(JSON.stringify(json));  //argues
}

lua:
function request( webjsonstr )
	local argues = JSON:decode( webjsonstr );
 	local retval = {
		account = "caozhenbiao",
		password = "1231231231"
	}
	return JSON:encode( retval );
end

