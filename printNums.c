#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "wrappers.h"

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
   return count;
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
}
