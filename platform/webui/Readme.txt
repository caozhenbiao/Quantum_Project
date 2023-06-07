添加QT依赖文件

打开QT C:\Windows\System32\cmd.exe /A /Q /K C:\Qt\Qt5.14.2\5.14.2\msvc2017\bin\qtenv2.bat

C:\Windows\System32\cmd

windeployqt webui.exe



new QWebChannel(qt.webChannelTransport, function(channel) {
	var cmd = channel.objects.command;
	cmd.show( true );
});

