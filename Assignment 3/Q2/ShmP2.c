#include <fcntl.h>
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
#define SNAME "/mysem"
#define SNAME1 "/mysem1"

struct my_data_struct
{
    int index;
    char str[7];
};

const char *semName = "asdfsd";
void printsems(sem_t *sem_work, sem_t *sem_no_work)
{

    int a = 10;
    int b = 10;
    sem_getvalue(sem_work, &a);
    sem_getvalue(sem_no_work, &b);
    printf("\n%s = %d\n", SNAME, a);
    printf("%s = %d\n", SNAME1, b);
}

int main(int argc, char *argv[])
{
    int i, shmid,ret;
    void *shared_memory;
    struct my_data_struct *shmp;
    struct my_data_struct idack[1];
    sem_t * sem_write_done = sem_open(SNAME, O_CREAT, 0777, 0);
    sem_t * sem_read_done = sem_open(SNAME1, O_CREAT, 0777, 0);
    shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("Shared memory");
    }

    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        perror("Shared memory attach");
    }
    int st = 0;
    while (st != 99)
    {
        printf("\n--------------------------------\nP2 waiting for data\n");
        ret =sem_wait(sem_write_done);//This ensures writer has written data.
        if(ret !=0) perror("reader loc1");

        shmp = (struct my_data_struct *)shared_memory;
        printf("\treceived data: \nindex --- string\n");
        for (i = 0; i < 5; i++)
        {
            printf("\t%d --- %s\n", shmp[i].index, shmp[i].str);
            if (shmp[i].index >= st)
            {
                st = shmp[i].index;
            }
        }
        idack[0].index = st;
        strcpy(idack[0].str , "got it");
        printf("P2 sending acknowledgement : %d\n",idack[0].index);
        memcpy(shared_memory, idack, sizeof(idack));
        if (st >= 49)
        {
            st = 99;
        }
        //printf("reader calling sem_post(sem_read_done)");
        ret = sem_post(sem_read_done);
        if(ret !=0) perror("reader loc3");
    }
    sem_close(sem_read_done);
    sem_close(sem_write_done);
    return 0;
}