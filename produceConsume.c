#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "wrappers.h"
#define COUNTMAX 10

pthread_mutex_t countMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t moreCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t lessCond = PTHREAD_COND_INITIALIZER;
int count = 0;
int countMax;

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
        Pthread_mutex_lock (&countMutex);
        while(count == countMax){
            //    printf("Thread %ld is waiting for countmax < 10", (long)tid);
            Pthread_cond_wait(&lessCond, &countMutex);
        } 
        assert(count < countMax);


        int amt  = countMax + 1;
        while((count + amt) > countMax){ 
            amt = rand() % (countMax) + 0;
        }
        count += amt;
        printf("Count is %d. Thread %ld is producing %d.\n", count, tid,  countMax - count);
        Pthread_cond_signal(&moreCond);
        Pthread_mutex_unlock(&countMutex);
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
        Pthread_mutex_lock (&countMutex);
        while(count == 0)
        {
            pthread_cond_wait(&moreCond, &countMutex);
        }
        assert(count > 0);

        int amt  = count;
        while((count - amt) > 0){ 
            amt = rand() % (countMax) + 0;
        }
        //printf("Count == %d Amt == %d", count,amt);
        count -= amt;
        if(count < 0) count == 0;

        printf("Count is %d. Thread %ld is consuming %d\n", count, tid,  count);
        Pthread_cond_signal(&lessCond);
        Pthread_mutex_unlock(&countMutex);   
    }
}

/* main
 * Creates one producer thread and one consumer thread.
 */
int main (int argc, char *argv[])
{
    if(argc == 7) {
        if(strcmp(argv[1],"-p") != 0 ||
                strcmp(argv[3],"-c") != 0 ||
                strcmp(argv[5],"-s") != 0 ||
                argc < 7){
            printf("Usage : produceConsume -p <numberOfproducers> -c <numberOfConsumers> -s <size>\n");
            pthread_exit(0);
        }
    } else {
        printf("Usage : produceConsume -p <numberOfproducers> -c <numberOfConsumers> -s <size>\n");
        pthread_exit(0);
    }
    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[4]);
    countMax = atoi(argv[6]);
    pthread_t prodThreads[numProducers];
    pthread_t consThreads[numConsumers];
    int i;
    for(i = 0; i < numProducers; i ++){
        Pthread_create(&prodThreads[i],NULL,produce,(void *) i);
    }
    for(i = numProducers; i  < (numProducers + numConsumers); i ++){ 
        Pthread_create(&consThreads[i - numProducers],NULL,consume,(void *) i);
    }
    for(i = 0; i < numProducers; i ++){
        void** retval;
        Pthread_join(prodThreads[i], retval);
    }

    for(i = numProducers; i  < (numProducers + numConsumers); i ++){ 
        void** retval;
        Pthread_join(consThreads[i], retval);
    }
    pthread_exit(0);
}


