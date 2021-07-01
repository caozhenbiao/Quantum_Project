
WEBCAM
 
 webcam INTERFACE:
	1. int connect(ip,port,name,password);
	2. int connect_privatekey(ip,port,name,keyfile);
	3. int request(handle,callback);
	    * function callback(responseText)
	4. int disconnect();



LUA EXAMPLE:

local webcamobj = require "webcam"
--webcamobj.addcam("pnpcam1",2,"192.168.1.255:32108;VSTC013535RKCST");
--webcamobj.addcam("TPLINKcam",1,"rtsp://192.168.1.108:554/stream2;60");
webcamobj.addcam("localcam",3,"1;50"); -- 本机摄像对序号;JPG质量
webcamobj.display(50,function(tag,frm,len)
	print("get frm:"..len);
	websockobj.sendfrm( frm,len);
	--local s = ofaceobj.framedetect( len, frm);
	--local nfrm, nlen = ofaceobj.imageProc( len, frm);
	--websockobj.sendfrm( nfrm,nlen);
	--for k, v in pairs( s ) do
	--	websockobj.sendstr( v );
	--end
end);


webcamobj.monitor( cmd.target, 300);  --fps
webcamobj.monitor( workcamname, 3 );
webcamobj.move( workcamname,cmd.direct,500);
