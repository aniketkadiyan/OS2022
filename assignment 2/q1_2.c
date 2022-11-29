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

  printf("Inside secondary thread\n");
  int x=0;
  x= system(" make -j$(nproc) -s");
  pthread_exit(NULL);
}

int main(int argc, char **argv)
{
  pthread_t thread;
  pthread_attr_t tattr;
  int rc=0;
  int t_priority =0;
  struct sched_param param;
  int policy;
  time_t start_clock, end_clock;
  void * stptr;
  
  printf("start main\n");
  rc = pthread_attr_init(&tattr);
  rc = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
  rc = pthread_attr_getschedparam(&tattr, &param);
  while (t_priority<90){
	  printf("priority = %d\n",t_priority);
	  param.sched_priority = t_priority;
	  rc = pthread_attr_setschedpolicy(&tattr , SCHED_FIFO);
	  clock_start = clock();
	  rc = pthread_create(&thread, &tattr, threadfunc, &param);
	  pthread_join(thread , &stptr);
	  clock_end = clock();
	  printf("FIFO time = %10.6f secs\n", (double)(clock_end-clock_start)/CLOCKS_PER_SEC);
	  rc = pthread_attr_setschedpolicy(&tattr , SCHED_RR);
	  clock_start = clock();
	  rc = pthread_create(&thread, &tattr, threadfunc, &param);
	  pthread_join(thread , &stptr);
	  clock_end = clock();
	  printf("RR time = %10.6f secs\n", (double)(clock_end-clock_start)/CLOCKS_PER_SEC);
	  rc = pthread_attr_setschedpolicy(&tattr , SCHED_OTHER);
	  clock_start = clock();
	  rc = pthread_create(&thread, &tattr, threadfunc, &param);
	  pthread_join(thread , &stptr);
	  clock_end = clock();
	  printf("Other time = %10.6f secs\n", (double)(clock_end-clock_start)/CLOCKS_PER_SEC);
	  t_priority +=20;
  }

  printf(" end main\n");
  return 0;
}