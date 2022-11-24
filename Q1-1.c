#define _MULTI_THREADED
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "check.h"

void *threadfunc(void *parm)
{
  printf("Inside secondary thread\n");
  sleep(5);  
  return NULL;
}

int main(int argc, char **argv)
{
  pthread_t thread;
  pthread_attr_t tattr;
  int rc=0;
  int t_priority =0;
  struct sched_param param;
  int policy;

  printf("Enter Testcase - %s\n", argv[0]);


  rc = pthread_attr_init(&tattr);
  rc = pthread_attr_getschedparam(&tattr, &param);
  param.sched_priority = t_priority;
  rc = pthread_attr_setschedpolicy(&tattr , SCHED_RR);
  rc = pthread_attr_setschedparam(&tattr, &param);
  rc = pthread_create(&thread, &tattr, threadfunc, NULL);
  rc = pthread_getschedparam(thread, &policy, &param);

  printf("The thread scheduling parameters indicate:\n"
         "policy = %d\n", policy);
  printf("priority = %d\n",
         param.sched_priority);

  printf("Main completed\n");
  return 0;
}