#ifndef _OS_TIMER_H
#define _OS_TIMER_H

#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
//#include "offcar_assistant.h"

#define SIGRTTIMER (39)

class OSTimer
{
	private:
		timer_t timer_id;
		int event;
		sem_t os_timer_thread_flag;
		pthread_t timer_thread_id;

	private:
		static void *_OSTimerThread(void * arg);
		void run();


	public:
		

		OSTimer();
		~OSTimer();

		/**
		 *	User must call this function in *main* thread, before using any other timer methods		 
		 */
		static void TimerEnvInit();
		/**
		 *	Start a one-shot timer.	 
		 */
		bool StartOneShotTimer(unsigned int u32_DelayInMS, int event);
		/**
		 *	Start a periodic timer.
		 */
		bool StartPeriodicTimer(unsigned int u32_DelayInMS, int event);

		/**
		 *	On timer callback. User should implement it. The event will be the value when user start a timer.
		 */
		virtual void on_timer(int event){return ;};

};


#endif
