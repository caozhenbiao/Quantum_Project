--*****LUA WEB CAMER SCRIPT*****--
print("load wscamer lua script success at :" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
local websockobj;
local webcamobj;
local workcamname;
local JSON = require "luajson"

--split string to table
function split(s, delim)
    if type(delim) ~= "string" or string.len(delim) <= 0 then
        return
    end
    local start = 1
    local t = {}
    while true do
    local pos = string.find (s, delim, start, true) -- plain find
        if not pos then
          break
        end
        table.insert (t, string.sub (s, start, pos - 1))
        start = pos + string.len (delim)
    end
    table.insert (t, string.sub (s, start))
    return t
end

--LUA数据表打印
key = ""
function PrintTable(table, level)
  level = level or 1
  local indent = ""
  for i = 1, level do
    indent = indent.."  "
  end
  if key ~= "" then
    print(indent..key.." ".."=".." ".."{")
  else
    print(indent .. "{")
  end
  key = ""
  for k,v in pairs(table) do
     if type(v) == "table" then
        key = k
        PrintTable(v, level + 1)
     else
        local content = string.format("%s%s = %s", indent .. "  ",tostring(k), tostring(v))
		print(content)
      end
  end
  print(indent .. "}")
end

 
function table2json(table, level)
  level = level or 1
  local retstr="";
  for k,v in pairs(table) do
     if type(v) == "table" then
        key = k
        retstr = retstr .. table2json(v, level + 1)
     else
        retstr = retstr .. tostring(v)
      end
  end
  return "[" .. retstr .. "]";
end


--检测结果转成WEBSOCk发送字符串
function detectresult2websockstring(classes,boxes)
	local sbox="";
	local sclasses = "";
	if classes ~= nil and boxes ~= nil then 
		local cnt =  string.len( classes )  /4 ;
		if cnt > 0 then 
			for i = 0, cnt-1 do
				local  vclass = string.sub(classes,i*4+1,i*4+4);
				local l = string.unpack("f", string.sub(boxes,i*4+1,i*4+4+4) );
				local t = string.unpack("f", string.sub(boxes,i*4+4+1,i*4+8+4) );
				local r = string.unpack("f", string.sub(boxes,i*4+8+1,i*4+12+4) );
				local b = string.unpack("f", string.sub(boxes,i*4+12+1,i*4+16+4) );
				sclasses = sclasses .. string.unpack("i", vclass ) .. ",";
				sbox = sbox .. string.format( "{'l':%d,'t':%d,'w':%d,'h':%d},", l,t,r-l,b-t);
			end
			sclasses = string.sub( sclasses, 1, -2 );
			sbox = string.sub( sbox, 1, -2 );	
		end 
	end
	return string.format("{'class':[%s],'box':[%s]}",sclasses,sbox);
end


 --LUA脚本宿主,安装事件处理函数
mainProc = coroutine.create(function(t)install(t,
	--命令处理(commsvr recv)
	function( addr, data, len )
	end,
	--定时器su
	function(timestamp)
		local filename = os.date("%H%M%S", os.time());
		local dtcfile  = "./webui/takephoto/" .. filename;
		local srcfile  = "./webui/takephoto/_" .. filename .. ".jpg";
		webcamobj.takephoto("localcam",srcfile);
		--local rtbl = yoloobj.filedetect(srcfile,dtcfile);
		--if #rtbl > 0 then
		--	PrintTable(rtbl);
		--end
		--os.remove(srcfile);
	end,
	--退出事件
	function()
		websockobj.destory();
		webcamobj.destory();
		print("EXIT")
	end)
end)

--websocket模块
websockproc = coroutine.create(function()
	websockobj = require "websock"
	websockobj.start("0.0.0.0",2980,function(evt,len,data)
		if evt == 1 then			
			if string.len( data ) > 0  then
				local cmd = JSON:decode( data );
				if cmd.action == "start" then
					webcamobj.monitor( cmd.target, 200);  --fps
					workcamname = cmd.target;
				elseif cmd.action == "stop" then
					webcamobj.monitor( workcamname, 3 );
					workcamname=nil;
				elseif cmd.action == "move" and workcamname ~= nil then
					webcamobj.move( workcamname,cmd.direct,500);
				end
			else
				webcamobj.monitor( workcamname, 0 );
				workcamname=nil;	
			end
		end
		if evt == 0 and workcamname ~= nil then
			webcamobj.monitor( workcamname, 200 );
		end
	end);
end)

--webcam模块
frmcnt = 0;
webcamProc = coroutine.create(function()
	print("webcamproc start")
	webcamobj = require "webcam"
	--webcamobj.addcam("TPLINKcam",1,"rtsp://192.168.3.6:554/stream1;80");
	--webcamobj.addcam("pnpcam1",2,"192.168.1.255:32108;VSTC013535RKCST");
	webcamobj.addcam("localcam",3,"0;256"); -- 本机摄像对序号;JPG质量
	--webcamobj.addcam("remotescreen",4,"192.168.3.17:1000"); 
	--webcamobj.addcam("remotescreen",4,"127.0.0.1:1000"); 
	--webcamobj.addcam("localscreen", 5 ,"0,100,1280,720");
	webcamobj.display(10,function(tag,frm,len)
		print( "recv frm buffer:" .. len );
		websockobj.sendfrm(frm,len);
		frmcnt = frmcnt + 1;
	end);
end)

--初始化信息设置
function webuiInit()
	--local localip = websockobj.getip("eth0");
	--return "{'svrip':'" .. localip .. "','webcam':['localcam','TPLINKcam','pnpcam1','remotescreen','localscreen']}";
	return "{'svrip':'127.0.0.1','webcam':['localcam']}";
end

--初始化文件夹
coroutine.resume(webcamProc);
coroutine.resume(websockproc);
coroutine.resume(mainProc,5000);
print("load main.lua finish")