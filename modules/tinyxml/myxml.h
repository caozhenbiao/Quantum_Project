#include "tinyxml.h"
#include <map>
class cmyxml{
public:
	int  load(const char * file );
    void destory();
	int RootElement();
	int FirstChildElement(int index, const char * n );
	int NextSiblingElement(int index, const char* n);
	const char* Attribute(int index, const char* n);
	const char* GetText(int index);
private:
	TiXmlDocument *  xmdoc;
	std::map<int,TiXmlElement *> mynodes;
	unsigned int nodeidx;
};
