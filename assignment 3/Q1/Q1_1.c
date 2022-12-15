#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5

void dine(int n);
pthread_t philosopher[NUM_PHILOSOPHERS];
pthread_mutex_t chopstick[NUM_FORKS];
int min(int a, int b);
int max(int a, int b);
sem_t Forks[NUM_FORKS];
int main()
{
  // Define counter var i and status_message
  int i, status_message;
  void *msg;

  // Initialise the semaphore array

  for (i = 1; i <= NUM_FORKS; i++)
  {
    sem_init(&Forks[i], 0, 1);
    status_message = pthread_mutex_init(&chopstick[i], NULL);
    // Check if the mutex is initialised successfully
    if (status_message == -1)
    {
      printf("\n Mutex initialization failed");
      exit(1);
    }
  }

  // Run the philosopher Threads using *dine() function
  for (i = 1; i <= NUM_PHILOSOPHERS; i++)
  {
    status_message = pthread_create(&philosopher[i], NULL, (void *)dine, i);
    if (status_message != 0)
    {
      printf("\n Thread creation error \n");
      exit(1);
    }
  }

  // Wait for all philosophers threads to complete executing (finish dining) before closing the program
  for (i = 1; i <= NUM_PHILOSOPHERS; i++)
  {
    status_message = pthread_join(philosopher[i], &msg);
    if (status_message != 0)
    {
      printf("\n Thread join failed \n");
      exit(1);
    }
  }

  // Destroy the chopstick Mutex array

  for (i = 1; i <= NUM_FORKS; i++)
  {
    sem_destroy(&Forks[i]);
    status_message = pthread_mutex_destroy(&chopstick[i]);
    if (status_message != 0)
    {
      printf("\n Mutex Destroyed \n");
      exit(1);
    }
  }
  return 0;
}
void printforks()
{
  int i = 0;
  for (i = 0; i < NUM_FORKS; i++)
  {
    printf("%d , ", Forks[i]);
  }
  printf("\n");
}

void dine(int n)
{
  int x = 3;
  while (x > -1)
  {
    printf("\nPhilosopher % d is thinking ", n);
    sleep((rand() % 5) + 1);

    int minc = n;
    int maxc = n + 1;
    if (maxc == NUM_FORKS + 1)
    {
      minc = 1;
      maxc = 5;
    }

    // pick up chopsticks in order
    int rc = sem_wait(&Forks[minc]);
    if (rc == 0)
    {
      printf("\n\tphilospher %d has fork %d", n, minc);
      rc = sem_wait(&Forks[maxc]);
      if (rc == 0)
      {
        printf("\n\tphilospher %d has fork %d", n, maxc);
      }
    }
    // printf("\nphilospher %d is picking forks %d and %d",n,minc,maxc);
    //  After picking up both the chopstick philosopher starts eating
    printf("\nPhilosopher % d is eating ", n);
    sleep(3);
    x--;
    printf("\nPhilosopher % d Finished eating ", n);

    // drop chopsticks in order
    sem_post(&Forks[minc]);
    sem_post(&Forks[maxc]);

    // printf("\nphilospher %d has put down forks %d and %d",n,minc,maxc);
  }
}
int min(int a, int b)
{
  return (a > b) ? b : a;
}
int max(int a, int b)
{
  return (a > b) ? a : b;
}
