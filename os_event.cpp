#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#include "os_event.h"

using namespace std;

OSEvent g_cOATaskEvent[OA_TASK_NUM] = {OA_INVALID_TASK, MAIN_TASK, MQTT_TASK, POWER_SWAP_TASK};
static char * s_taskName[] = {"INVALID_TASK", "MAIN_TASK", "MQTT_TASK", "POWER_SWAP_TASK"};

OSEvent::OSEvent(TASK_INDEX_T task_id)
{
    m_taskID = task_id;
    m_taskEvent.events = 0;
    pthread_mutex_init(&m_taskEvent.events_mutex, NULL);
    pthread_cond_init(&m_taskEvent.events_cond, NULL);
}

bool OSEvent::os_event_send(TASK_INDEX_T task_id, unsigned32 event)
{
    bool status = true;

    if(event == 0) {
        //please don't use 0 as event, still we will treat it as valid and return true
    }
    else {
        if((task_id > OA_INVALID_TASK) && (task_id < OA_TASK_NUM)) {
            PRINT_DEBUG("Send task to %d(%s) task.", task_id, s_taskName[task_id]);
            g_cOATaskEvent[task_id].MUTEX_LOCK();
            g_cOATaskEvent[task_id].event_add(event);
            g_cOATaskEvent[task_id].COND_SIGNAL();
            g_cOATaskEvent[task_id].MUTEX_UNLOCK();
        }
        else {
            PRINT_ERROR("Send to a invalid task_id(%d).", task_id);
            status = false;
        }
    }
    return status;
}
unsigned32 OSEvent::os_event_wait()
{
    unsigned32 events;

    MUTEX_LOCK();

    /* Is there an event already there? */
    while(m_taskEvent.events == 0) {
        COND_WAIT();
    }

    events = m_taskEvent.events;
    PRINT_DEBUG("Task %d(%s) recv events(%d).", m_taskID, s_taskName[m_taskID], events);
    m_taskEvent.events = 0;
    MUTEX_UNLOCK();

    return events;
}
int OSEvent::MUTEX_LOCK()
{
    return(pthread_mutex_lock(&m_taskEvent.events_mutex));
}
int OSEvent::MUTEX_UNLOCK()
{
    return(pthread_mutex_unlock(&m_taskEvent.events_mutex));
}

void OSEvent::event_add(unsigned32 event)
{
    m_taskEvent.events |= event;
}

int OSEvent::COND_WAIT()
{
    return (pthread_cond_wait(&m_taskEvent.events_cond, &m_taskEvent.events_mutex));
}
int OSEvent::COND_SIGNAL()
{
    return (pthread_cond_signal(&m_taskEvent.events_cond));
}
string OSEvent::os_event_rc_get()
{
    string return_code;
    MUTEX_LOCK();
    return_code = m_sRetCode;
    m_sRetCode = "";
    MUTEX_UNLOCK();
    return return_code;
}
void OSEvent::os_event_rc_set(TASK_INDEX_T task_id, string return_code)
{
    if((task_id > OA_INVALID_TASK) && (task_id < OA_TASK_NUM)) {
        PRINT_DEBUG("Set return code(%s) to %d(%s) task.", return_code.c_str(), task_id, s_taskName[task_id]);
        g_cOATaskEvent[task_id].MUTEX_LOCK();
        g_cOATaskEvent[task_id].m_sRetCode = return_code;
        g_cOATaskEvent[task_id].MUTEX_UNLOCK();
    }
    return;
}
