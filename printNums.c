#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrappers.h"
pthread_cond_t myconvar = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int global = 0;
int numThreads = 0;

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
    int mult = 0;    
    pthread_mutex_lock(&mymutex); //acquires the lock 
    while(global < 100)
    {
        global = (int)threadNumber + 1 + (mult * numThreads); //updates global 
        printf("Thread %d = %d\n", (int)threadNumber, global); //print updated global
        mult ++; 
        pthread_cond_signal(&myconvar); 
        if(global < 100) pthread_cond_wait(&myconvar,&mymutex); //
    }

    pthread_mutex_unlock(&mymutex); //releases the lock
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
