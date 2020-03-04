#include<semaphore.h>
void unixError(char * msg);

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
int Pthread_join(pthread_t thread, void** retval);
int Pthread_mutex_destroy(pthread_mutex_t* myMutex);
int Pthread_cond_wait(pthread_cond_t* myCond, pthread_mutex_t* myMutex);
int Pthread_cond_signal(pthread_cond_t* myCond);
int Sem_init(sem_t* sem, int a, int b);
int Sem_wait(sem_t* sem);
int Sem_post(sem_t* sem);
int Sem_destroy(sem_t* sem);
