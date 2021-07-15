
dlib_19.22

http://dlib.net/compile.html


在linux 下编译 dlib-19.17 
cmake . -DDLIB_NO_GUI_SUPPORT=OFF
make


linux/ubuntu
cd examples
mkdir build
cd build
cmake ..
cmake --build . --config Release

win
cmake -G "Visual Studio 14 2015 Win64" -T host=x64 ..
cmake ..


g++ -std=c++11 -O3 -I.. ../dlib/all/source.cpp -lpthread -lX11 example_program_name.cpp
sudo apt-get install libx11-dev




angshuai@wangshuai-TUFx470:~$ locate libcurl.so.4
/home/wangshuai/Downloads/zjuvpn/curl-7.47.0/lib/.libs/libcurl.so.4
/home/wangshuai/Downloads/zjuvpn/curl-7.47.0/lib/.libs/libcurl.so.4.4.0
/usr/lib/x86_64-linux-gnu/libcurl.so.4
/usr/lib/x86_64-linux-gnu/libcurl.so.4.4.0
/usr/local/lib/libcurl.so.4
/usr/local/lib/libcurl.so.4.4.0
wangshuai@wangshuai-TUFx470:~$ ll /usr/local/lib/libcurl.so.4
lrwxrwxrwx 1 root root 16 10月 14 17:40 /usr/local/lib/libcurl.so.4 -> libcurl.so.4.4.0*
wangshuai@wangshuai-TUFx470:~$ sudo rm -rf /usr/local/lib/libcurl.so.4
wangshuai@wangshuai-TUFx470:~$ sudo ln -s /usr/lib/x86_64-linux-gnu/libcurl.so.4.4.0 /usr/local/lib/libcurl.so.4 
wangshuai@wangshuai-TUFx470:~$ ll /usr/local/lib/libcurl.so.4
lrwxrwxrwx 1 root root 42 10月 20 21:10 /usr/local/lib/libcurl.so.4 -> /usr/lib/x86_64-linux-gnu/libcurl.so.4.4.0
wangshuai@wangshuai-TUFx470:~$ cmake 
————————————————
 

opencv linux 安装注意
一定要在linux目录下进行编译，在/mnt/hgfa共享目录下编译不成功


cmake -D BUILD_DOCS=OFF -D WITH_QT=OFF -D WITH_XINE=OFF -D WITH_OPENGL=ON -D WITH_TBB=ON -D WITH_OPENNI=ON -D BUILD_EXAMPLES=OFF


