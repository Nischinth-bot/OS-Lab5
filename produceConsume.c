#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "wrappers.h"


pthread_mutex_t countMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t moreCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t lessCond = PTHREAD_COND_INITIALIZER;
int count = 0;
int countMax = 10;

/* produce
 * This code is executed in a thread.  The thread
 * increments the count variable up to COUNTMAX. 
 * It signals a potentially waiting consumer.
 */
void *produce(void *threadid)
{
   long tid = (long)threadid;
   while (1) //loop forever
   {
      pthread_mutex_lock (&countMutex);
      if (count == COUNTMAX)
         pthread_cond_wait(&lessCond, &countMutex);
      printf("Count is %d. Thread %ld is producing %d.\n", count, tid,  COUNTMAX - count);
      assert(count < COUNTMAX);
      count = COUNTMAX;
      pthread_cond_signal(&moreCond);
      pthread_mutex_unlock(&countMutex);
   }
}

/* consume
 * This code is executed in a thread.  The thread
 * decrements the count variable down to COUNTMAX. 
 * It signals a potentially waiting producer.
 */
void *consume(void *threadid)
{
   long tid = (long)threadid;
   while (1) //loop forever
   {
      pthread_mutex_lock (&countMutex);
      if (count == 0)
         pthread_cond_wait(&moreCond, &countMutex);
      printf("Count is %d. Thread %ld is consuming %d\n", count, tid,  count);
      assert(count > 0);
      count = 0;
      pthread_cond_signal(&lessCond);
      pthread_mutex_unlock(&countMutex);
   }
}

/* main
 * Creates one producer thread and one consumer thread.
 */
int main (int argc, char *argv[])
{
   pthread_t producer;
   pthread_t consumer;
   long t0 = 0, t1 = 1;
   Pthread_create(&producer, NULL, produce, (void *)t0);
   Pthread_create(&consumer, NULL, consume, (void *)t1);
   pthread_exit(0);
}


