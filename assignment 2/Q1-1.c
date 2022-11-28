#define _MULTI_THREADED
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
# include <time.h>
#include <unistd.h>

#define BILLION  1000000000L;

int i=2;

void *threadfunc(void *parm)
{
	struct timespec start, stop;
double accum;
  printf("Inside secondary thread\n");
  int x=0;
  int y = 4294967295
  while(x<y){
	  x=x+1;
  }
accum = ( stop.tv_sec - start.tv_sec )+ (double)( stop.tv_nsec - start.tv_nsec ) / (double)BILLION;
printf( "%lf\n", accum );
  pthread_exit(&i)
}

int main(int argc, char **argv)
{
  pthread_t thread;
  pthread_attr_t tattr;
  int rc=0;
  int t_priority =0;
  struct sched_param param;
  int policy;

  rc = pthread_attr_init(&tattr);
  rc = pthread_attr_getschedparam(&tattr, &param);
  param.sched_priority = t_priority;
  rc = pthread_attr_setschedpolicy(&tattr , SCHED_RR);
  rc = pthread_attr_setschedparam(&tattr, &param);
  rc = pthread_create(&thread, &tattr, threadfunc, &param);
  rc = pthread_getschedparam(thread, &policy, &param);
	int *ptr;
pthread_join(thread , (void**)&ptr)


 rc = pthread_attr_init(&tattr);
  rc = pthread_attr_getschedparam(&tattr, &param);
  param.sched_priority = t_priority;
  rc = pthread_attr_setschedpolicy(&tattr , SCHED_FIFO);
  rc = pthread_attr_setschedparam(&tattr, &param);
  rc = pthread_create(&thread, &tattr, threadfunc, &param);
  rc = pthread_getschedparam(thread, &policy, &param);
	int *ptr;
pthread_join(thread , (void**)&ptr)

 rc = pthread_attr_init(&tattr);
  rc = pthread_attr_getschedparam(&tattr, &param);
  param.sched_priority = t_priority;
  rc = pthread_attr_setschedpolicy(&tattr , SCHED_OTHER);
  rc = pthread_attr_setschedparam(&tattr, &param);
  rc = pthread_create(&thread, &tattr, threadfunc, &param);
  rc = pthread_getschedparam(thread, &policy, &param);
	int *ptr;
pthread_join(thread , (void**)&ptr)
  printf("Main completed\n");
  return 0;
}
