#include <stdio.h>
#include "snapc.h"
int main(int argc, char * argv[]) {
	csnapc dev;
	if( argc < 2 ) 
		dev.play("0.0.0.0:1000;0,0,640,480");
	else
		dev.play(argv[1]);
	while ( 1 ) {
		printf("cmd>");
		char sz[256] = { 0 };
		if (fgets(sz, 256, stdin) == NULL)
			continue;
		if (strcmp(sz, "exit\n") == 0)
			break;
	}
	dev.stop();
	return 0;
}