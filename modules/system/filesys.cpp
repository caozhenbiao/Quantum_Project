#include "filesys.h"
#ifdef _WIN32
#define ACCESS _access   
#define MKDIR(a) _mkdir((a)) 
#else
#include <stdarg.h>   
#include <sys/stat.h> 
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include <sys/fcntl.h>
#define sprintf_s snprintf
#endif

int getFiles(const char * cate_dir, vector<string> & v ){
#ifdef _WIN32
	_finddata_t file;
	int lf = -1;
	if ((lf=_findfirst(cate_dir,&file)) != -1){
		do{
			if(strcmp(file.name,".") == 0 || strcmp(file.name,"..")==0)
				continue;
			v.push_back(file.name);
		}while(_findnext(lf, &file) == 0);
	}
	_findclose(lf);
#else
	/*
	DIR *dir;
	struct dirent *ptr;
	char path[256]={0};
	char ext[32]={0};
	sscanf(cate_dir,"%[^*]*%s", path, ext);
	printf("%s  path:%s  ext:%s\n",cate_dir, path, ext );
	if ((dir=opendir(path)) == NULL){
		perror("Open dir error...");
		return 0;
	}
	while ((ptr=readdir(dir)) != NULL){
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0 || !strstr(ptr->d_name,ext))  ///current dir OR parrent dir
		    continue;
		else
			v.push_back( ptr->d_name);
	}
	closedir(dir);
	*/
#endif
	return v.size();
}

//设置数据存储路径
int createpath( const char* path ){
	char folder[256]; 
	sprintf_s(folder,sizeof(folder),"%s/",path);
	for( size_t i=1; i<strlen(folder); i++){ 
		if( folder[i]=='/' || folder[i]=='\\'){ 
			folder[i] = 0; 
#ifdef _WIN32
			if(ACCESS(folder,0) != 0){
				if(MKDIR(folder)==-1){  
#else
			if (access(folder, 0) != 0) {
				if (mkdir(folder,0755) == -1) {
#endif
					perror("mkdir error");
					return -1;
				} 
			} 
			folder[i] = '/';  
		} 
	}
	return 0;
}