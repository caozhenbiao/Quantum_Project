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


1：安装samba
sudo apt-get update
sudo apt-get install samba

2：创建一个目录并给权限
创建目录：sudo mkdir /home/test/share
给创建的这个目录设置权限
sudo chmod 777 /home/test/share

3：修改samba配置文件，设置为共享目录
sudo nano /etc/samba/smb.conf

在配置文件smb.conf的最后添加下面的内容：
[share]
comment = share folder
browseable = yes
path = /home/test/share
create mask = 0700
directory mask = 0700
valid users = test
force user = test
force group = test
public = yes
available = yes
writable = yes

4：重启samba服务器
sudo service smbd restart

5：在win10电脑上打开文件资源管理器输入
\\ + Ubuntu电脑的IP地址访问共享文件夹


