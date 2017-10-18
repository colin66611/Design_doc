#ifndef OS_EVENT_H
#define OS_EVENT_H

#include <string>
#include "oa_type.h"

using namespace std;

typedef struct {
    pthread_mutex_t     events_mutex;   
    pthread_cond_t      events_cond;   
    unsigned32          events;
}PER_TASK_EVENT_T;

typedef enum {
    OA_INVALID_TASK,
    MAIN_TASK = 1,
    MQTT_TASK = 2,
    POWER_SWAP_TASK = 3,
    OA_TASK_NUM

} TASK_INDEX_T;

class OSEvent {
    private:
        PER_TASK_EVENT_T m_taskEvent;
        TASK_INDEX_T m_taskID;
        string m_sRetCode;

    public:
        OSEvent(TASK_INDEX_T task_id);

        int MUTEX_LOCK();
        int MUTEX_UNLOCK();
        
        unsigned32 event_get();
        void event_add(unsigned32 event);
        
        int COND_WAIT();
        int COND_SIGNAL();

        static bool os_event_send(TASK_INDEX_T task_id, unsigned32 event);
        unsigned32 os_event_wait();

        string os_event_rc_get();
        void os_event_rc_set(TASK_INDEX_T task_id, string return_code);

};

extern OSEvent g_cOATaskEvent[OA_TASK_NUM];
#endif
