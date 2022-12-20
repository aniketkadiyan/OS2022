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
void printsems(sem_t * sem_work,sem_t * sem_no_work)
{

    int a=10;
    int b=10;
    sem_getvalue(sem_work,&a);
    sem_getvalue(sem_no_work,&b);
    printf("\n%s = %d\n",SNAME,a);
    printf("%s = %d\n",SNAME1,b);
}
void parent(void)
{

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
    //printf("Key of shared memory in parent is %d\n", shmid);
    sem_t * sem_write_done = sem_open(SNAME, O_CREAT, 0777, 0);
    sem_t * sem_read_done = sem_open(SNAME1, O_CREAT, 0777, 0);
    //printsems(sem_write_done, sem_read_done);
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1)
    {
        perror("Shared memory attach");
    }
    int st = 0;
    while (st != 99)
    {
        // printf(">>>>>>>>>>writing data<<<<<<<<<\n");
        for (i = 0; i < 5; i++)
        {
            tstrings[i].index = i + st;
            strcpy(tstrings[i].str, strarr[i+st]);
            //printf("sent: %d --- %s\n", tstrings[i].index, tstrings[i].str);
        }
        memcpy(shared_memory, tstrings, 80);
        sem_post(sem_write_done);
        
        sem_wait(sem_read_done);
        shmp = (struct my_data_struct *)shared_memory;
        if(4+st !=shmp[0].index)
        {
            printf("incorrect acknoledgement id st=%d, ack=%d",st,shmp[0].index);
            exit(EXIT_FAILURE);
        }
        st = shmp[0].index;

        printf("\n>>>>>>>>>> ack id %d received<<<<<<<<<\n", st);
        st++;
        if (st >= 49)
        {
            st = 99;
        }
    }
    sem_close(sem_read_done);
    sem_close(sem_write_done);
    return;
}

void child(void)
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
        ret =sem_wait(sem_write_done);//This ensures writer has written data.
        if(ret !=0) perror("reader loc1");

        shmp = (struct my_data_struct *)shared_memory;
        //printf("\treceived data: \n");
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
}

int main(int argc, char *argv[])
{
    if(sem_unlink(SNAME)!=0)perror("main loc1");
    if(sem_unlink(SNAME1)!=0)perror("main loc2");

    sem_t * sem_write_done = sem_open(SNAME, O_CREAT, 0777, 0);
    sem_t * sem_read_done = sem_open(SNAME1, O_CREAT, 0777, 0);
    if(sem_write_done == SEM_FAILED) printf("sem_open(SNAME failed");
    if(sem_read_done == SEM_FAILED) printf("sem_open(SNAME1 failed");

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
    }
    else
    {
        int status;
        parent();
    }
    printf("\n\n\texiting program\n\n");
    sem_close(sem_read_done);
    sem_close(sem_write_done);
    if(sem_unlink(SNAME)!=0) perror("main loc3");
    if(sem_unlink(SNAME1)!=0) perror("main loc4");

    return 0;
}