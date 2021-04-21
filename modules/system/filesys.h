#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef linux
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#endif
#include <string>
#include <vector>
using namespace std;
int getFiles(const char* cate_dir, vector<string> & v );
int createpath( const char* path );
