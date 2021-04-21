#include "cammgr.h"
#include "onvifpr.h"
#include "pnppr.h"
#include "camera.h"
#include "snaps.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

mbase::mbase(){
	m_playevt  = NULL;
	m_frmevt  = NULL;
	memset(store_path,0x00,256);
	memset(identify,0x00,256);
 	memset(m_source,0x00,256);
	m_bsave = false;
	bdisplay = false;;
	width = 640;
	hight = 480;
	m_status = 0;
}
mbase::~mbase(){

}
size_t mbase::getframe(ftype ftype, std::string & frm){
	if( 0 != event_wait( m_frmevt ) )
		return 0;
	event_reset(m_frmevt);
	frm += m_frmdata;
	m_frmdata.clear();
	event_set(m_frmevt);
	return frm.size();
}

void mbase::setframe(const char* frm, int size){
	if( 0 != event_wait( m_frmevt ) )
		return;
	event_reset(m_frmevt);
	m_frmdata.clear();
	m_frmdata.assign(frm, size);
	event_set(m_frmevt);
}

void mbase::move(int dir, int tms){
}

void mbase::setsave(bool save ){
	m_bsave = save;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//创建摄像头
int ccammgr::add(const char* mark, const char* url, int type){
	mbase * mbs = NULL;
	switch( type ){
	case 1: mbs = new convifpr(); break;
	case 2: mbs = new cpnppr();  break;
	case 3: mbs = new ccamera(); break;
	case 4: mbs = new csnaps(); break;
	}
	memset(mbs->m_source,0x00,256);
	strncpy(mbs->m_source,url,strlen(url));
	strncpy(mbs->identify,mark,strlen(mark));
	insert(std::make_pair(mark, mbs));
	return 0;
}

//管理器退出
int ccammgr::del(const char* mark){
	for(std::map<std::string,mbase*>::iterator it = this->begin(); it!=this->end(); it++){
		it->second->stop();
		delete it->second;
	}
	return 0;
}

//play
int ccammgr::play(const char* mark, bool store){
	std::map<std::string,mbase*>::iterator ifnd = this->find( mark );
	if( ifnd != end() ){
		ifnd->second->play(store);
	}
	return 0 ;
}

//stop
int ccammgr::stop(const char* mark){
	std::map<std::string,mbase*>::iterator ifnd = this->find( mark );
	if( ifnd != end() ){
		ifnd->second->stop();
	}
	return 0;
}

//getfrm
size_t ccammgr::getfrm(const char* mark,ftype type,std::string & frm ){
	std::map<std::string,mbase*>::iterator ifnd = this->find( mark );
	if( ifnd != end() ){
		return ifnd->second->getframe(type,frm );
	}
	return 0;
}

//move
int ccammgr::move(const char* mark,int x, int y){
	std::map<std::string,mbase*>::iterator ifnd = this->find( mark );
	if( ifnd != end() ){
		ifnd->second->move(x,y);
	}
	return 0;
}

//Setsave
int ccammgr::storage(const char* mark,bool save){
	std::map<std::string,mbase*>::iterator ifnd = this->find( mark );
	if( ifnd != end() ){
		ifnd->second->setsave( save );
	}
	return 0;
}
