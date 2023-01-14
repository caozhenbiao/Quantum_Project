--*****LUA WEB CAMER SCRIPT*****--
print("load wscamer lua script success at :" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local webcamobj;
websockobj = require "websock"

--webcam模块
webcamProc = coroutine.create(function()
	webcamobj = require "webcam"
	--webcamobj.addcam("pnpcam1",2,"192.168.1.255:32108;VSTC013535RKCST");
	--webcamobj.addcam("TPLINKcam",1,"rtsp://192.168.1.108:554/stream2;60");
	webcamobj.addcam("localcam",3,"0;50"); -- 本机摄像对序号;JPG质量
	webcamobj.display(41,function(tag,frm,len)
		print("get frm:"..len);
	end);
end)


coroutine.resume(webcamProc);
