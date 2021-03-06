#ifndef _HIK_EVENT_H_    
#define _HIK_EVENT_H_   
#ifdef _MSC_VER
#define event_handle void *   
#else
#include <pthread.h>   
typedef struct _event_t{
    bool state;  
    bool manual_reset;  
    pthread_mutex_t mutex;  
    pthread_cond_t cond;  
}event_t;
#define event_handle event_t*   
#endif   
//返回值：NULL 出错   
event_handle event_create(bool manual_reset, bool init_state);  
//返回值：0 等到事件，-1出错   
int event_wait(event_handle hevent);  
//返回值：0 等到事件，1 超时，-1出错   
int event_timedwait(event_handle hevent, long milliseconds);  
//返回值：0 成功，-1出错   
int event_set(event_handle hevent);  
//返回值：0 成功，-1出错   
int event_reset(event_handle hevent);  
//返回值：无   
void event_destroy(event_handle hevent);  
#endif
