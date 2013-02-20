#ifndef __H_LOCKS__
#define __H_LOCKS__
#include <pthread.h>

struct lock {
	pthread_mutex_t mutex;
	
};

struct lock* get_lock();
void enter_critical_section(struct lock* a_lock);
void exit_critical_section(struct lock* a_lock);

#endif
