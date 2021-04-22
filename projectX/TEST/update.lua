print("update app files success at :" .. os.date("%Y-%m-%d %H:%M:%S", os.time()))
if os.getenv("OS") == "Windows_NT" then
	print("unable run in windows system")
	os.execute("copy ..\\..\\platform\\quark\\output\\quark.exe .\\run\\");
	os.execute("copy ..\\..\\platform\\quark\\liblua\\win32\\pthread.dll .\\run\\")
	os.execute("copy ..\\..\\platform\\quark\\liblua\\output\\luadll.dll .\\run\\")
	os.execute("copy ..\\..\\modules\\webcam\\output\\webcam.dll .\\run\\")
	os.execute("copy ..\\..\\modules\\opencv\\output\\opencv.dll .\\run\\")
	os.execute("copy ..\\..\\modules\\ssh\\output\\ssh.dll .\\run\\")
	os.execute("copy ..\\..\\platform\\lualibs\\luajson.lua .\\run\\")
	os.execute("copy ..\\..\\platform\\lualibs\\luaxapi.lua .\\run\\")
	os.execute("copy ..\\..\\platform\\lualibs\\luamd5.lua .\\run\\")
	os.execute("copy ..\\..\\platform\\lualibs\\luabit.lua .\\run\\")
else
	os.execute("cp ../../smartx/bin/smartx_linux.exe ./smartx.exe");
	os.execute("cp ../../modules/webcam/bin/webcam.so ./")
	os.execute("cp ../../modules/opencv/bin/opencv.so ./")
	os.execute("cp ../../modules/darknet/bin/darknet.so ./")
	os.execute("cp ../../smartx/libs/lualib/luajson.lua ./run/")
	os.execute("cp ../../smartx/libs/lualib/luaxapi.lua ./run/")
	os.execute("cp ../../smartx/libs/lualib/luamd5.lua ./run/")
	os.execute("cp ../../smartx/libs/lualib/luabit.lua ./run/")
end
os.exit();

