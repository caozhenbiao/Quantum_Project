#include "myxml.h"

int cmyxml::load(const char * file ){
	nodeidx = 0;
	xmdoc = new TiXmlDocument;
	if( xmdoc->LoadFile(file)){
		mynodes[0] =  xmdoc->RootElement();
		return 0;
	}else{
		delete xmdoc;
		xmdoc = NULL;
		return -1;
	}
}

void cmyxml::destory(){
	mynodes.clear();
	nodeidx = 0;
	xmdoc->Clear();
	delete xmdoc;
}

int cmyxml::FirstChildElement( int index, const char * n ){
	TiXmlElement * r =   mynodes[index]->FirstChildElement( n );
	if( r ){
		mynodes[++nodeidx] =  r;
		return nodeidx;
	}
	return -1;
}

int  cmyxml::NextSiblingElement(int index, const char* n){
	TiXmlElement * r =  mynodes[index]->NextSiblingElement( n );
	if( !r )
		return -1;
	mynodes[++nodeidx] =  r;
	return nodeidx;
}

const char* cmyxml::Attribute(int index, const char* n){
	const char *   r  = mynodes[index]->Attribute( n );
	return r;
}

const char* cmyxml::GetText(int index){
	const char *   r  = mynodes[index]->GetText();
	return r;
}

