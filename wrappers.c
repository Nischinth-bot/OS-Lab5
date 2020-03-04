#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "wrappers.h"

/* unixError
 * Called when an error occurs to print an error message and exit.
 */
void unixError(char * msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/* Pthread_create
 * Wrapper for pthread_create.
 * The pthread_create() function starts a new thread in the calling process.  The
 * new thread starts execution by invoking start_routine(); arg is passed as  the
 * sole argument of start_routine().
 * The attr argument points to a pthread_attr_t structure whose contents are used
 * at thread creation time to determine  attributes  for  the  new  thread.
 * Before returning, a successful call to pthread_create() stores the ID  of  the
 * new  thread  in  the  buffer  pointed to by thread.
 * On success, pthread_create() returns 0; on error, it returns an error  number,
 * and the contents of *thread are undefined.
 */
int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
        void *(*start_routine)(void *), void *arg)
{
    int rc = pthread_create(thread, attr, start_routine, arg);
    if (rc != 0)
    {
        unixError("pthread_create error");
    }
    return rc;
}

int Pthread_join(pthread_t thread, void** retval){
    int rc = pthread_join(thread, retval); 
    if (rc != 0)
    {
        unixError("pthread_join error");
    }
    return rc;
}


int Pthread_mutex_destroy(pthread_mutex_t* mutex){
    int rc = pthread_mutex_destroy(mutex);
    if (rc != 0)
    {
        unixError("pthread_mutex_destroy error");
    }
    return rc;

}


int Pthread_mutex_lock(pthread_mutex_t* mutex){
    int rc = pthread_mutex_lock(mutex);
    if (rc != 0)
    {
        unixError("pthread_mutex_lock error");
    }
    return rc;
}



int Pthread_mutex_unlock(pthread_mutex_t* mutex){
    int rc = pthread_mutex_unlock(mutex);
    if (rc != 0)
    {
        unixError("pthread_mutex_unlock error");
    }
    return rc;
}

int Pthread_cond_wait(pthread_cond_t * myCond, pthread_mutex_t * myMutex){

    int rc = pthread_cond_wait(myCond, myMutex);
    if (rc != 0)
    {
        unixError("pthread_cond_wait error");
    }
    return rc;
}


int Pthread_cond_signal(pthread_cond_t * myCond){

    int rc = pthread_cond_signal(myCond);
    if (rc != 0)
    {
        unixError("pthread_cond_signal error");
    }
    return rc;
}


int Sem_init(sem_t* sem, int a, int b){
    int rc = sem_init(sem,a,b);
    if(rc != 0)
    {
        unixError("sem_init error");
    }
    return rc;

}

int Sem_destroy(sem_t* sem){
    int rc = sem_destroy(sem);
    if(rc != 0)
    {
        unixError("sem_detroy error");
    }
    return rc;

}

int Sem_wait(sem_t* sem){
    int rc = sem_wait(sem);
    if(rc != 0)
    {
        unixError("sem_wait error");
    }
    return rc;

}

int Sem_post(sem_t* sem){
    int rc = sem_post(sem);
    if(rc != 0)
    {
        unixError("sem_post error");
    }
    return rc;
}
