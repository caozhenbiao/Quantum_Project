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

void buzzer( unsigned int freq, unsigned int duration );
void getOsInfo(std::string& os_name, std::string& version);
void getCpuInfo(int& freq, unsigned int& guid, std::string& mid, std::string& type);
void getMemoryInfo(int& decimal_total, int& decimal_avl);
void getDiskInfo(const char* path, unsigned int& total, unsigned int& free);
std::string getAuthorize( char * prikey );
int  authenticate();