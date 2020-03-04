#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "wrappers.h"

/*
 * Add comments here to indicate the number of threads that should be used so
 * that your threaded program has the best performance on array with this number
 * of elements:
 * 10G
 * 1G
 * 100M
 */

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
void getArgs(int argc, char * argv[], int * size, int * threads);
void printUsage();
void printArray(int * array, int size);
void initArray(int ** array, int size);
void threadedSum(void* i);
int sequentialSum(int * array, int size);
int DEBUG = 0;
//global variables used by threads.
int* array;
int size;
int sum = 0;
int indx = 0;
int threadsFinished = 0;
int numThreads;
/*
 * printUsage
 * This function is called if the user provides bad command
 * line arguments.  It outputs usage information and
 * exits.
 */
void printUsage()
{
    printf("usage: sumArray -s <size> -t <threads> [-D]\n");
    printf("-s <size> - size of array\n");
    printf("-t <threads> - number of threads used for sum\n");
    printf("-D - optional debug flag\n");
    exit(1);
}

/*
 * getArgs
 * This function parses the command line arguments and
 * initializes the ints pointed to by size and threads
 * @params: argc - count of command line arguments
 * @params: argv - array of command line arguments
 * @params: size - pointer to an int variable to be set to
 *          the atoi of the command line arg that follows the
 *          "-s" in the array.
 * @params: threads - pointer to an int variable to be set to
 *          the atoi of the command line arg that follows the
 *          "-t" in the array.
 */
void getArgs(int argc, char * argv[], int * size, int * threads)
{
    int i;
    if (argc != 5 && argc != 6) printUsage();
    for (i = 1; i < argc; i++)
    {
        if (strcmp("-s", argv[i]) == 0)
        {
            (*size) = atoi(argv[i + 1]);
            i++;
        } else if (strcmp("-t", argv[i]) == 0)
        {
            (*threads) = atoi(argv[i + 1]);
            i++;
        } else if (strcmp("-D", argv[i]) == 0)
            DEBUG = 1;
        else
            printUsage(); 
    }
    if ((*size) <= 0 || (*threads) <= 0) printUsage();
}

/* initArray
 * Calls malloc to dynamically allocate space for an int array
 * of with ``size" elements
 * Uses rand() to randomly generate initial values for the
 * array elements. Elements are set to a value in the range 
 * 0 to 9
 * @param: array - pointer to the array pointer
 * @param: size - number of elements to be created
 */
void initArray(int ** array, int size)
{
    int i;
    int * ptr; 
    srand(time(0));
    (*array) = malloc(sizeof(int) * size);
    ptr = (*array);
    for (i = 0; i < size; i++)
        ptr[i] = rand() % 10;
}

/* threadedSum
 * Sums up all elements of the array
 * @param: i - Thread number
 */
void threadedSum(void* threadNum)
{
    int tn = (int) threadNum;
    int startIndex = tn * (size/numThreads);
    int endIndex = startIndex + (size/numThreads);
    int i;
    int thisSum = 0;
    for(i = startIndex; i < endIndex; i ++){
        thisSum += array[i];
    }
    Pthread_mutex_lock(&myMutex);
    sum += thisSum;
    
    // if threads dont equally divide entire array, add the remaining elementts
    // at the end
    if(tn == numThreads - 1){
        if(size % numThreads != 0){
            if(numThreads % 2 == 0){
                sum += array[size - 2];
                sum += array[size - 1];
            }
            else{
                sum += array[size - 1];
                }
        }
    }
    Pthread_mutex_unlock(&myMutex);
    pthread_exit(0);
}


int sequentialSum(int * array, int size)
{
    int i = 0, sum = 0;
    for (i = 0; i < size; i++)
        sum += array[i];
    return sum;
}


/* printArray
 * prints the elements of the array, 20 per line.
 * @param: array - point to first element in array
 * @param: size - number of elements in the array
 */
void printArray(int * array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if ((i % 20) == 0) printf("\n");
        printf("%3d", array[i]);
    }
}

/* main
 * Driver for the program. Creates an array, initializes
 * the elements of the array, and calls the sequentialSum
 * to add up all elements of the array.  Command line arguments
 * are given to provided to indicate the size of the array and
 * the number of threads to use to calculate the sum
 * using threads.
 */
int main(int argc, char *argv[])
{
    struct timespec begin, end;
    double elapsed;
    int threads;

    /*****************************SEQUENTIAL SECTION*********************************/


    getArgs(argc, argv, &size, &threads);   
    initArray(&array, size);
    if (DEBUG) printArray(array, size);
    // get clock time before and after the sum
    clock_gettime(CLOCK_MONOTONIC, &begin); 
    int seqSum = sequentialSum(array,size);
    clock_gettime(CLOCK_MONOTONIC, &end);
    //calculate the difference and convert to seconds
    elapsed = end.tv_sec - begin.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("\nSequential sum is: %d\n", seqSum);
    printf("Cpu time: %lf seconds\n", elapsed);

    // *****************************THREADED SECTION*******************************8*/
    int i;
    numThreads = threads;
    pthread_t myThreads[threads];

    //get clock time before and after the sum 
    clock_gettime(CLOCK_MONOTONIC, &begin); 
    for(i = 0; i < threads; i ++){
        //printf("Creating thread %d\n", i);
        Pthread_create(&myThreads[i], NULL, threadedSum, (void*) i);
    }
    //sleep(1);
    for(i = 0; i < threads; i ++){
        // printf("Joining thread %d\n", i);
        Pthread_join(myThreads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    //calculate the difference and convert to seconds
    elapsed = end.tv_sec - begin.tv_sec;
    elapsed += (end.tv_nsec - begin.tv_nsec) / 1000000000.0;
    printf("\nThreaded sum is: %d\n", sum);
    printf("Cpu time: %lf seconds\n", elapsed);

    return 0;
}
