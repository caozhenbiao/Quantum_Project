Quantum Project (量子计划）
跨平台模块化开发框架，支持windows/linux/mac等平台。

2021/4/20

1. platform
   平台相关服务程序及桌面程序

2. modules
    各类模块，包括mysql,opencv,sermon,sqlite,ssh等
    模块调试
    在vs中设置调试
    quark.exe -s main.lua

3. projectX
    各类项目文件夹

4. thirdlibs
    模块开发所需要的第三方库

5.tools
    压缩及认证工具

6. utils
    杂项及备份






**************************项目编译、更新、打包***********************
在各个项目止录下，有相应的make.lua文件，该脚本直持当前项目的编译、更新、打包任务。
Lua make.lua [args]
args
build   : 编译所有项目所需要执行文件及依赖库等
update  : 更新所有项目所需要要文件至当前run目录
package : 项目打包

其它说明：
加密算法
	1:单机+时间
	 提供设备识别码，
	2:企业识别号


Sermon 需要安装如下依赖
Apt install flex
Apt install bison

/libcap
Make
Make install


