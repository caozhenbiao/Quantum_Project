local INI = {}

--读取行数据   返回 当前行数据  剩余数据
function INI:ReadLine(data)
	local lenInByte = #data;
	local nEndPos = 0;
	local retline = nil;  --当前行数据
	local retData = nil;  --剩余数据
	for i=1,lenInByte do
		local curByte = string.byte(data, i)
        	local byteCount = 1;
		if curByte>0 and curByte<=127 then
			byteCount =  1;
			if curByte == 10 then
				nEndPos = i;
				break;
			end
		elseif curByte>=192 and curByte<223 then
			byteCount =  2;
		elseif curByte>=224 and curByte<239 then
			byteCount = 3;
		elseif curByte>=240 and curByte<=247 then
			byteCount =  4;
		end
		i = i + byteCount - 1;
	end
 
	if nEndPos == 0 then
		retline = data;
	else
		retline = string.sub(data,1,nEndPos);
		retData = string.sub(data,nEndPos+1,lenInByte);
	end
	--去掉两端的空字符
	retline = retline:match("%s*(.-)%s*$");  
	return retline,retData;
end

function INI:ReadIniFile(tData,IniFileName)
	local curSection = nil;
    local file = assert(io.open(IniFileName,"rb"));
    local data = file:read("*a"); -- 读取所有内容
	file:close();
	while(nil~=data) 
	do
		line,data = ReadLine(data);
		if  nil ~= line  and string.len(line)>0 and string.byte(line,1) ~= 35 then
			tmpSection = string.match(line,"%[([%w]*)%]")
			if tmpSection ~= nil then
				curSection = tmpSection;
			else
				item = string.match(line,"%s*(%w+)%s*=");
				itemData = string.match(line,"%s*=%s*(.*)$");
				if tData[curSection] == nil then
                   			tData[curSection] = {};
               			end
				tData[curSection][item] =   itemData ;
			end
		end
	end
	return tData;
end

function WriteIniFile(tData,IniFileName)
	local iniData = "";
	for Section, vSection in pairs(tData) do  
		iniData = iniData .. "[" .. Section .."]\r\n"; 
		for key, value in pairs(vSection) do  
			iniData = iniData ..key .. "=" .. value .. "\r\n"; 
		end
	end 
	local file = assert(io.open(IniFileName,"wb"));
	file:write(iniData);
	file:close();
end

function INI:GetPrivateProfileString(szSection,szKey,szDefault,IniFileName)
	local retVal =  szDefault;
	local tData  = {};
	ReadIniFile(tData,IniFileName);
	if tData[szSection] ~= nil then
		if tData[szSection][szKey] ~= nil then
			retVal = tData[szSection][szKey];
		end
	end
	return retVal;
end

function INI:GetPrivateProfileInt(szSection,szKey,nDefault,IniFileName)
	local retVal =  nDefault;
	retVal = tonumber(GetPrivateProfileString(szSection,szKey,nDefault,IniFileName));
	return retVal;
end

function INI:WritePrivateProfileString(szSection,szKey,szValue,IniFileName)
	local tData  = {};
	ReadIniFile(tData,IniFileName);
	if tData[szSection] == nil then
	   tData[szSection] = {};
	end
	tData[szSection][szKey] = szValue ;
	WriteIniFile(tData,IniFileName);
end

function INI:new(args)
   local new = { }
   if args then
      for key, val in pairs(args) do
         new[key] = val
      end
   end
   return setmetatable(new, INI)
end

INI.__index = INI
return INI:new()