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


HTML 文件传输

//上传文件方法
function UpladFile() {
    var fileObj = document.getElementById("FIFILE_ImageFile").files[0]; // js 获取文件对象
    var url =  "http://127.0.0.1:8006" + "/webui/upload/"; // 接收上传文件的后台地址
    var form = new FormData(); // FormData 对象
    form.append("file", fileObj); // 文件对象
    xhr = new XMLHttpRequest();  // XMLHttpRequest 对象
    xhr.open("UPFILE", url, true); //post方式，url为服务器请求地址，true 该参数规定请求是否异步处理。
    xhr.onload = function(evt){
    	alert("1上传完成！");
    }; 
    xhr.onerror =  function(evt){
    	alert("1上传失败！");
    }; 
    xhr.upload.onprogress = function(evt){
        var progressBar = document.getElementById("progressBar");
	    var percentageDiv = document.getElementById("percentage");
	    // event.total是需要传输的总字节，event.loaded是已经传输的字节。如果event.lengthComputable不为真，则event.total等于0
	    if (evt.lengthComputable) {//
	        progressBar.max = evt.total;
	        progressBar.value = evt.loaded;
	        percentageDiv.innerHTML = Math.round(evt.loaded / evt.total * 100) + "%";
	    }
	    var time = document.getElementById("time");
	    var nt = new Date().getTime();//获取当前时间
	    var pertime = (nt-ot)/1000; //计算出上次调用该方法时到现在的时间差，单位为s
	    ot = new Date().getTime(); //重新赋值时间，用于下次计算
	    var perload = evt.loaded - oloaded; //计算该分段上传的文件大小，单位b
	    oloaded = evt.loaded;//重新赋值已上传文件大小，用以下次计算
	    //上传速度计算
	    var speed = perload/pertime;//单位b/s
	    var bspeed = speed;
	    var units = 'b/s';//单位名称
	    if(speed/1024>1){speed = speed/1024;units = 'k/s';}
	    if(speed/1024>1){speed = speed/1024;units = 'M/s';}
	    speed = speed.toFixed(1);
	    //剩余时间
	    var resttime = ((evt.total-evt.loaded)/bspeed).toFixed(1);
	    //time.innerHTML = '，速度：'+speed+units+'，剩余时间：'+resttime+'s';
	    if(bspeed==0) time.innerHTML = '上传已取消';
    };
    xhr.upload.onloadstart = function(){//上传开始执行方法
        ot = new Date().getTime();   //设置上传开始时间
        oloaded = 0;//设置上传开始时，以上传的文件大小为0
   };
   xhr.send(form); //开始上传，发送form数据
}
