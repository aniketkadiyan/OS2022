#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define NUM_BOWLS 2
#define EATING 10
#define THINKING 11

void dine(int n);
pthread_t philosopher[NUM_PHILOSOPHERS];

int min(int a, int b);
int max(int a, int b);
sem_t Forks[NUM_FORKS];
sem_t bowls[1];

int main()
{
  // Define counter var i and rc
  int i, rc, ret;
  void *msg;

  // Initialise the semaphore array
  for (i = 0; i < NUM_FORKS; i++)
  {
    ret = sem_init(&Forks[i], 0, 1);
    if (ret != 0)
    {
      perror("Error while sem_init(&Forks");
      exit(EXIT_FAILURE);
    }
  }
  for (i = 0; i < 1; i++)
  {
    ret = sem_init(&bowls[i], 0, NUM_BOWLS);
    if (ret != 0)
    {
      perror("Error while sem_init bowls");
      exit(EXIT_FAILURE);
    }
  }

  // Run the philosopher Threads using *dine() function
  for (i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    rc = pthread_create(&philosopher[i], NULL, (void *)dine, i);
  }

  // Wait for all philosophers threads to complete executing
  for (i = 0; i < NUM_PHILOSOPHERS; i++)
  {
    rc = pthread_join(philosopher[i], &msg);
  }

  // Destroy the fork Mutex array
  for (i = 0; i < NUM_FORKS; i++)
  {
    sem_destroy(&Forks[i]);
  }
  for (i = 0; i < NUM_BOWLS; i++)
  {
    sem_destroy(&bowls[i]);
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
  int stat = THINKING;
  while (x < 4)
  {
    printf("\nPhilosopher % d is thinking ", n);
    sleep((rand() % 5) + 1);

    int minc = n;
    int maxc = n + 1;
    if (maxc == NUM_FORKS)
    {
      maxc = 0;
    }

    // pick up forks in order
    int bno = -1;
    // printf("\n\t\t philospher %d waiting for a bowl", n);
    if (sem_wait(&bowls[0]) == 0)
    {
      printf("\n\t\ta bowl acquired by philospher %d", n);

     
        if (sem_trywait(&Forks[maxc]) == 0)
        {
          printf("\n\tphilospher %d has fork %d", n, maxc);
      
        }
        if (sem_trywait(&Forks[minc]) == 0)
        {
          printf("\n\tphilospher %d has fork %d", n, minc);
         
        }
      
    }
    else
    {
      perror("Error while acquiring bowl");
      exit(EXIT_FAILURE);
    }

    //  After picking up both the fork philosopher starts eating
    printf("\nPhilosopher % d is eating ", n);
    sleep(3);
    x--;
    printf("\nPhilosopher % d Finished eating ", n);

    // drop forks in order
    sem_post(&Forks[minc]);
    sem_post(&Forks[maxc]);
    printf("\n\tphilospher %d has released fork %d", n, minc);
    printf("\n\tphilospher %d has released fork %d", n, maxc);

    sem_post(&bowls[0]);
    printf("\n\t\ta bowl released by philospher %d", n);
    stat = THINKING;

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
