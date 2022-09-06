
#include <iostream>
#include <string.h>



#include <iostream>
#include <string>

class cmyssl{
public:
	cmyssl();
	~cmyssl();
 
public:
	char* getPeerCert( const char* ip, unsigned short port);
 
};
