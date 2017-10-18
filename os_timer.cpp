/*! @file os_timer.cpp
 * Description: The timer feature.
 *
 * By: Colin Song
 *
 * Copyright (c) 2016 NIO. All rights reserved.
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "os_timer.h"
#include "oa_main.h"

OSTimer::OSTimer()
{
    struct sigevent sig_event;
    int rc = -1;

    sem_init(&os_timer_thread_flag, 0, 0);

    pthread_create(&timer_thread_id, NULL, _OSTimerThread, (void * )this);

    sig_event.sigev_notify = SIGEV_SIGNAL;
    sig_event.sigev_signo = SIGRTTIMER;
    sig_event.sigev_value.sival_int = 0;
    sig_event.sigev_notify_attributes = NULL;

    rc = timer_create(CLOCK_MONOTONIC, &sig_event, &timer_id);
    if(rc != 0)
    {
        PRINT_ERROR("timer_create failed with return value %d.", rc);
    }

    //wait until timer_thread ready
    sem_wait(&os_timer_thread_flag);
    sem_destroy(&os_timer_thread_flag);
}

void OSTimer::TimerEnvInit()
{
    sigset_t set;
    int rc = -1;

    sigemptyset(&set);
    sigaddset(&set, SIGRTTIMER);

    rc = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if(rc !=0 )
    {
        PRINT_ERROR("TimerEnvInit failed with errno(%d).", errno);
    }

}

bool OSTimer::StartOneShotTimer(unsigned int u32_DelayInMS, int event)
{
    bool ret = false;
    int rc = -1;
    struct itimerspec i_timer_spec;

    this->event |= event;

    i_timer_spec.it_value.tv_sec = u32_DelayInMS / 1000;
    i_timer_spec.it_value.tv_nsec = 0;

    rc = timer_settime(timer_id, 0, &i_timer_spec, NULL);
    if(rc == -1)
    {
        PRINT_ERROR("timer_settime failed with errno(%d).", errno);
        ret = false;
    }
    else
    {
        ret = true;
    }

    return ret;
}

bool OSTimer::StartPeriodicTimer(unsigned int u32_DelayInMS, int event)
{
    bool ret = false;
    int rc = -1;
    struct itimerspec i_timer_spec;

    this->event |= event;

    i_timer_spec.it_value.tv_sec = u32_DelayInMS / 1000;
    i_timer_spec.it_value.tv_nsec = 0;

    i_timer_spec.it_interval.tv_sec = i_timer_spec.it_value.tv_sec;
    i_timer_spec.it_interval.tv_nsec = i_timer_spec.it_value.tv_nsec;

    rc = timer_settime(timer_id, 0, &i_timer_spec, NULL);
    if(rc == -1)
    {
        PRINT_ERROR("timer_settime failed with errno(%d).", errno);
        ret = false;
    }
    else
    {
        ret = true;
    }

    return ret;
}

void OSTimer::run()
{
    sigset_t set, pendmask;
    //struct sigaction sigact;
    siginfo_t info;
    int rc;

    sigemptyset(&set);
    sigaddset(&set, SIGRTTIMER);

    if (sigpending(&pendmask) < 0)
    {
        perror("sigpending error");
    }
    while (sigismember(&pendmask, SIGRTTIMER))
    {
        PRINT_TRACE("SIGRTTIMER pending/n");
        rc=sigwaitinfo(&set, &info);
        if (sigpending(&pendmask) < 0)
        {
            perror("sigpending error");
        }
    }

    sem_post(&os_timer_thread_flag);

    while(1)
    {
        rc = sigwaitinfo(&set, &info);
        if(rc != -1)
        {
            this->on_timer(event);
            this->event = 0;
        }
    }
}

void* OSTimer::_OSTimerThread(void * arg)
{
    OSTimer * os_timer = (OSTimer *)arg;
    os_timer->run();

    return NULL;
}
