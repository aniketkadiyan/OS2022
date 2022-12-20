#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define SHM_KEY 0x1236


struct my_data_struct
{
    int index;
    char *str;
};


sem_t sem1, semparent, semchild;

void parent(void)
{

    sem_wait(&semparent);
    int i, shmid;
    void *shared_memory;
    struct my_data_struct *shmp;
    struct my_data_struct tstrings[5];
    char *strarr[50] = {"str 01", "str 02", "str 03", "str 04", "str 05", "str 06", "str 07", "str 08", "str 09", "str 10","str 11", "str 12", "str 13", "str 14", "str 15", "str 16", "str 17", "str 18", "str 19", "str 20","str 21", "str 22", "str 23", "str 24", "str 25", "str 26", "str 27", "str 28", "str 29", "str 30","str 31", "str 32", "str 33", "str 34", "str 35", "str 36", "str 37", "str 38", "str 39", "str 40","str 41", "str 42", "str 43", "str 44", "str 45", "str 46", "str 47", "str 48", "str 49", "str 50"};
    shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Shared memory");
    }
    printf("Key of shared memory in parent is %d\n", shmid);
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        perror("Shared memory attach");
    }
    printf("parent process attatch at : %p\n", shared_memory);
    int st = 0;
    while (st != 99)
    {
        sem_wait(&sem1);
        // printf(">>>>>>>>>>writing data<<<<<<<<<\n");
        for (i = 0; i < 5; i++)
        {
            tstrings[i].index = i + st;
            tstrings[i].str = strarr[i+st];
            printf("sent: %d --- %s\n", tstrings[i].index, tstrings[i].str);
        }
        memcpy(shared_memory, tstrings, 80);
        printf("\n>>>>>>>>>>data written<<<<<<<<<\n");
        sem_post(&sem1);
        sleep(5);   
        sem_wait(&sem1);
        // printf(">>>>>>>>>>receiveng ack id<<<<<<<<<\n");
        shmp = (struct my_data_struct *)shared_memory;
        st = shmp[0].index;
        
        printf("\n>>>>>>>>>> ack id %d received<<<<<<<<<\n", st);
        st++;
        if (st >= 49)
        {
            st = 99;
        }
        sem_post(&sem1);
    }
    // printf(">>>>>exiting parent<<<<<\n");
    sem_post(&semparent);
    // printf("byep\n");
    return;
}

void child(void)
{
    sem_wait(&semchild);
    int i, shmid;
    void *shared_memory;
    struct my_data_struct *shmp;
    struct my_data_struct idack[1];
    shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Shared memory");
    }

    printf("Key of shared memory in child is %d\n", shmid);
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        perror("Shared memory attach");
    }
    printf("Child Process attached at %p\n", shared_memory);
    sleep(3);
    int st = 0;
    while (st != 99)
    {
        sem_wait(&sem1);
        shmp = (struct my_data_struct *)shared_memory;
        printf("\treceived data: \n");
        for (i = 0; i < 5; i++)
        {
            printf("\t%d --- %s\n", shmp[i].index, shmp[i].str);
            if (shmp[i].index >= st)
            {
                st = shmp[i].index;
            }
        }
        idack[0].index = st;
        idack[0].str = "received";
        memcpy(shared_memory, idack, sizeof(idack));
        if (st >= 49)
        {
            st = 99;
        }
        sem_post(&sem1);
        sleep(5);
    }
    // printf(">>>>>exiting child<<<<<\n");
    sem_post(&semchild);
    // printf("byec\n");
}

int main(int argc, char *argv[])
{
    sem_init(&sem1, 1, 1);
    sem_init(&semparent, 1, 1);
    sem_init(&semchild, 1, 1);
    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (!pid)
    {
        child();
        // printf("Child   : Done with sem_open \n");
    }
    else
    {
        int status;
        parent();
        // wait(&status);
        // printf("Parent  : Done with sem_open \n");
    }
    // printf("exitm");
    sleep(3);
    sem_wait(&semparent);
    sem_wait(&semchild);
    printf("\n\n\texiting program\n\n");
    sem_post(&semparent);
    sem_post(&semchild);
    sem_close(&sem1);
    sem_close(&semparent);
    sem_close(&semchild);

    return 0;
}