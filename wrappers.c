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

