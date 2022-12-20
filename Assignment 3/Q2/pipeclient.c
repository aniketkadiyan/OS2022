#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "my_fifo"

int main()
{
    int st = 0, index = 0;
    while (st == 0)
    {
        int fifo = open(FIFO_NAME, O_RDONLY);
        char inmessage[512];
        read(fifo, inmessage, 512);
        printf("\t P2 Received message:\nstring : id\n%s\n", inmessage);

        close(fifo);
        fifo = open(FIFO_NAME, O_WRONLY);

        char *message1 = inmessage + (strlen(inmessage) - 3);
        index = atoi(message1);
        if (index >= 49)
        {
            st = 1;
        }
        printf("\tP2 sending acknowledgement : %d\n\t>>><<<\n", index);
        write(fifo, message1, 512);
        close(fifo);
    }
    return 0;
}