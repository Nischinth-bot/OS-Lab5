#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrappers.h"
pthread_cond_t myconvar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int global = 0;
int numThreads = 0;
int nextThread = 1;
int first = 0;
/* getThreadCount
 * parse the command line arguments to get the number
 * of threads to be created by this process.
 * argv[1] should contain positive decimal number
 * @param: argc - number of command line args
 * @param: argv[] - array of command line args
 */
int getThreadCount(int argc, char * argv[])
{
    int count;
    if (argc < 2 || (count = atoi(argv[1])) <= 0)
    {
        printf("usage: printNums <thread count>\n");
        exit(1);
    }
    numThreads = count;
    return count;
}

/**
 * printNumbers
 * Thread function
 */
void* printNumbers(void* threadNumber){ 
    pthread_mutex_lock(&mymutex); //acquires the lock         
    int mult = 0;    
    int threadNum = (int) threadNumber;
   // printf("Thread %d got the signal\n", threadNum);
    while(global < 10)
    {

        while((threadNum) != (nextThread - 1) && global < 10) {  
          //   printf("Waiting on thread = %d %d\n", threadNum, nextThread);
             pthread_cond_wait(&myconvar,&mymutex); 
            // printf("Resuming thread = %d\n", threadNum);
        }
        if(global < 10) {    //Double check if global still needs updating
            global = threadNum + 1 + (mult * numThreads);
            fprintf(stderr,"Thread %d = %d\n", (int)threadNumber, global); 
            nextThread ++; 
            if(nextThread > numThreads) nextThread = 1; //Reset the nextThread varible, if necessary.
            mult ++; 
        }
     //   printf("Work done for thread %d, sending signal\n",threadNum);
        pthread_cond_signal(&myconvar); /**Thread finishes its work for this 
                                        iteration and signals other threads waiting on convar.**/
        pthread_mutex_unlock(&mymutex); //releases the lock
    }
    pthread_mutex_unlock(&mymutex); //releases the lock
    pthread_mutex_destroy(&mymutex);
    pthread_exit(NULL);
}



/*
 * main 
 * call getThreadCount go get a count of the number of 
 * threads to create. Create the threads to print the numbers.
 * Use a condition variable to synchronize the output.
 */
int main (int argc, char *argv[])
{
    int threadCount = getThreadCount(argc, argv);
    pthread_t threads[threadCount];
    if (threadCount > 0)
    {
        int j;
        for(j = 0; j < threadCount; j ++){
            pthread_create(&threads[j], NULL, printNumbers, (void *) j);
        }
    }
    pthread_exit(NULL);
}
