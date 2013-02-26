#include <stdlib.h>
#include <locks.h>

struct lock* get_lock() 
{
	struct lock* retval;
	pthread_mutexattr_t attr;

	retval = (struct lock *) malloc(sizeof(struct lock)); 
	/* create a recursive lock */ 
	pthread_mutex_init(&retval->mutex, NULL);
	return retval;
}

void enter_critical_section(struct lock* a_lock)
{

	pthread_mutex_lock(&a_lock->mutex);
}

void exit_critical_section(struct lock* a_lock)
{ 

	pthread_mutex_unlock(&a_lock->mutex);
}
