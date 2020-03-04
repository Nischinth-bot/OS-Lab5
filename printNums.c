#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrappers.h"
#define MAXOUT 1000
pthread_cond_t myconvars[MAXOUT]  = {PTHREAD_COND_INITIALIZER};
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
    Pthread_mutex_lock(&mymutex); //acquires the lock         
    int mult = 0;    
    int threadNum = (int) threadNumber;
    while(global < MAXOUT)
    {
        while((threadNum) != (nextThread - 1)  && global != MAXOUT) {  
            Pthread_cond_wait(&myconvars[threadNum],&mymutex); 
        }
        if(global < MAXOUT) {    //Double check if global still needs updating
            global = threadNum + 1 + (mult * numThreads);
            fprintf(stderr,"Thread %d = %d\n", (int)threadNumber, global); 
            nextThread ++; 
            if(nextThread > numThreads) nextThread = 1; //Reset the nextThread varible, if necessary.
            mult ++; 
            Pthread_cond_signal(&myconvars[nextThread - 1]); 
        }
        int i;
        for(i = 0; i < numThreads ; i ++){
            Pthread_cond_signal(&myconvars[i]);  
        }
    }
    Pthread_mutex_unlock(&mymutex); //releases the lock
    //Pthread_mutex_destroy(&mymutex);
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
            Pthread_create(&threads[j], NULL, printNumbers, (void *) j);
        }
    }
    pthread_exit(NULL);
}
