
mkdir .\run
copy /Y ..\..\platform\quark\output\quark.exe .\run\
copy /Y ..\..\platform\quark\lib3th\output\pthread.dll .\run\
copy /Y ..\..\platform\quark\liblua\output\luadll.dll .\run\
copy /Y ..\..\modules\gmssl\output\gmssl.dll .\run\
copy /Y ..\..\modules\gmssl\libs\GmSSL\bin\libcrypto-1_1.dll .\run\
copy /Y ..\..\modules\gmssl\libs\GmSSL\bin\libssl_1_1.dll .\run\
copy /Y ..\..\modules\regedit\output\regedit.dll .\run\
copy /Y ..\..\modules\system\output\system.dll .\run\
copy /Y ..\..\modules\sqlite\output\sqlite.dll .\run\
copy /Y ..\..\platform\lualibs\luajson.lua .\run\
copy /Y ..\..\platform\lualibs\luaxapi.lua .\run\
copy /Y ..\..\platform\lualibs\luamd5.lua .\run\
copy /Y ..\..\platform\lualibs\luabit.lua .\run\
copy /Y ..\..\modules\opencv\output\opencv.dll .\run\

mkdir .\browser
copy /Y ..\..\platform\webui\output\pthread.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Core.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Gui.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Network.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Positioning.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5PrintSupport.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Qml.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5QmlModels.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Quick.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5QuickWidgets.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5WebChannel.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5WebEngineCore.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5WebEngineWidgets.dll .\browser\
copy /Y ..\..\platform\webui\output\Qt5Widgets.dll .\browser\
copy /Y ..\..\platform\webui\output\QtWebEngineProcess.exe .\browser\
copy /Y ..\..\platform\webui\output\webui.exe .\browser\

mkdir .\browser\platforms
copy /Y ..\..\platform\webui\output\platforms\qwindows.dll .\browser\platforms\

mkdir .\browser\resources
copy /Y ..\..\platform\webui\output\resources\icudtl.dat .\browser\resources\
copy /Y ..\..\platform\webui\output\resources\qtwebengine_devtools_resources.pak .\browser\resources\
copy /Y ..\..\platform\webui\output\resources\qtwebengine_resources.pak .\browser\resources\
copy /Y ..\..\platform\webui\output\resources\qtwebengine_resources_100p.pak .\browser\resources\
copy /Y ..\..\platform\webui\output\resources\qtwebengine_resources_200p.pak .\browser\resources\

mkdir .\browser\styles
copy /Y ..\..\platform\webui\output\styles\qwindowsvistastyle.dll .\browser\resources\


	


