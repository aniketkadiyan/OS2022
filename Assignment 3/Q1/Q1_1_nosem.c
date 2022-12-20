#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define n 5
#define DINNER_COMPLETED 10

pthread_t philosopher[n];
struct fork
{
    int taken;
} Forks[n];

struct philospher
{
    int left;
    int right;
} Pstat[n];

void dine(int id)
{

    printf("Philosopher %d is thinking\n", id + 1);
    sleep((rand() % 5) + 1);
    int numeats = 3;
    while (numeats > 0)
    {
        // specify left and right forks for id
        int lf = id;
        int rf = id + 1;
        if (rf == n)
        {
            rf = 0;
        }
        // if already completed dinner go to thinking
        if (Pstat[id].left == DINNER_COMPLETED && Pstat[id].right == DINNER_COMPLETED)
        {

            // printf("\tdinner already completed case for P%d\n", id + 1);
            printf("Philosopher %d is thinking\n", id + 1);
            sleep((rand() % 5) + 1);
            Pstat[id].left = Pstat[id].right = 0;
            printf("Philosopher %d is finished thinking\n", id + 1);
        }

        // if not completed dinner
        // if just taken two forks
        else if (Pstat[id].left == 1 && Pstat[id].right == 1)
        {
            sleep(3);
            // printf("\tjust got forks case for P%d\n", id + 1);
            printf("Philosopher %d just completed his dinner\n", id + 1);

            Pstat[id].left = Pstat[id].right = DINNER_COMPLETED; // remembering that he completed dinner by assigning value 10

            Forks[lf].taken = Forks[rf].taken = 0; // releasing forks
            printf("\tPhilosopher %d released fork %d and fork %d\n", id + 1, lf + 1, rf + 1);
            numeats--;
        }

        // left fork already taken, trying for right fork
        else if (Pstat[id].left == 1 && Pstat[id].right == 0)
        {
            sleep(2);
            // printf("\t got left fork but not right case for P%d\n", id + 1);

            if (Forks[rf].taken == 0)
            {
                Forks[rf].taken = Pstat[id].right = 1;
                printf("\tFork %d taken by Philosopher %d\n", rf + 1, id + 1);
            }
            else
            {
                printf("\tPhilosopher %d is waiting for Fork %d\n", id + 1, rf + 1);
            }
        }

        // no fork taken yet
        else if (Pstat[id].left == 0)
        {
            sleep(2);
            // printf("\t got no forks case for P%d\n", id + 1);
            // philospher 5 case
            if (id == (n - 1))
            {
                // philospher 5 has right fork
                if (Forks[rf].taken == 1 && Pstat[id].right == 1)
                {
                    // if left fork is free
                    if (Forks[lf].taken == 0)
                    {
                        Forks[lf].taken = Pstat[id].left = 1;
                        printf("\tFork %d taken by philosopher %d\n", lf + 1, id + 1);
                    }
                    // philospher 5 waiting for left fork
                    else
                    {
                        printf("\tPhilosopher %d is waiting for fork %d\n", id + 1, lf + 1);
                    }
                }
                // someone else has right fork
                else if (Forks[rf].taken == 1 && Pstat[id].right == 0)
                {
                    printf("\tPhilosopher %d is waiting for fork %d\n", id + 1, rf + 1);
                }
                // right fork is free but not with philospher 5
                else if (Forks[rf].taken == 0 && Pstat[id].right == 0)
                {
                    Forks[rf].taken = Pstat[id].right = 1;
                    printf("\tFork %d taken by philosopher %d\n", rf + 1, id + 1);
                }
            }
            // except last philosopher case
            else
            {
                // left fork is free
                if (Forks[lf].taken == 0)
                {
                    Forks[lf].taken = Pstat[id].left = 1;
                    printf("\tFork %d taken by Philosopher %d\n", lf + 1, id + 1);
                }
                // left fork is not
                else
                {
                    printf("\tPhilosopher %d is waiting for Fork %d\n", id + 1, lf + 1);
                }
            }
        }

        else
        {
        }
    }
    return;
}

int main()
{
    int i = 0, status;
    void *msg;
    for (i = 0; i < n; i++)
        Forks[i].taken = Pstat[i].left = Pstat[i].right = 0;

    // Run the philosopher Threads using *dine() function
    for (i = 0; i < n; i++)
    {
        status = pthread_create(&philosopher[i], NULL, (void *)dine, i);
        if (status != 0)
        {
            printf("\n Thread creation error \n");
            exit(1);
        }
    }

    // Wait for all philosophers threads to complete executing (finish dining) before closing the program
    for (i = 0; i < n; i++)
    {
        status = pthread_join(philosopher[i], &msg);
        if (status != 0)
        {
            printf("\n Thread join failed \n");
            exit(1);
        }
    }

    return 0;
}
