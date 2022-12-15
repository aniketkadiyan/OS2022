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
pthread_mutex_t chopstick[NUM_FORKS];
int min(int a, int b);
int max(int a, int b);
sem_t Forks[NUM_FORKS];
sem_t bowls[NUM_BOWLS];

int main()
{
  // Define counter var i and rc
  int i, rc;
  void *msg;

  // Initialise the semaphore array
  for (i = 1; i <= NUM_FORKS; i++)
  {
    sem_init(&Forks[i], 0, 1);
  }
  for (i = 1; i <= NUM_BOWLS; i++)
  {
    sem_init(&bowls[i], 0, 1);
  }

  // Run the philosopher Threads using *dine() function
  for (i = 1; i <= NUM_PHILOSOPHERS; i++)
  {
    rc = pthread_create(&philosopher[i], NULL, (void *)dine, i);
  }

  // Wait for all philosophers threads to complete executing
  for (i = 1; i <= NUM_PHILOSOPHERS; i++)
  {
    rc = pthread_join(philosopher[i], &msg);
  }

  // Destroy the chopstick Mutex array
  for (i = 1; i <= NUM_FORKS; i++)
  {
    sem_destroy(&Forks[i]);
  }
  for (i = 1; i <= NUM_BOWLS; i++)
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
  while (x > -1)
  {
    printf("\nPhilosopher % d is thinking ", n);
    sleep((rand() % 5) + 1);

    int minc = n;
    int maxc = n + 1;
    if (maxc == NUM_FORKS + 1)
    {
      maxc = 1;
    }

    // pick up chopsticks in order
    int bno = -1;
    int rc = sem_wait(&Forks[minc]);
    if (rc == 0)
    {
      printf("\n\tphilospher %d has fork %d", n, minc);
      rc = sem_wait(&Forks[maxc]);
      if (rc == 0)
      {
        printf("\n\tphilospher %d has fork %d", n, maxc);
        rc = 1;
        while (rc == 1)
        {
          if (sem_trywait(&bowls[1]) == 0)
          {
            printf("\n\t\tbowl 1 acquired by philospher %d", n);
            bno = 1;
            rc=-1;
          }
          else if (sem_trywait(&bowls[2]) == 0)
          {
            printf("\n\t\tbowl 2 acquired by philospher %d", n);
            bno = 2;
            rc = -1;
          }
        }
      }
    }

    //  After picking up both the chopstick philosopher starts eating
    printf("\nPhilosopher % d is eating ", n);
    sleep(3);
    x--;
    printf("\nPhilosopher % d Finished eating ", n);

    // drop chopsticks in order
    sem_post(&Forks[minc]);
    sem_post(&Forks[maxc]);
     printf("\n\tphilospher %d has released fork %d", n, minc);
     printf("\n\tphilospher %d has released fork %d", n, maxc);

    sem_post(&bowls[bno]);
    printf("\n\t\tbowl %d released by philospher %d", bno, n);
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
