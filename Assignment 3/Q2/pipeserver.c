#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define FIFO_NAME "my_fifo"
#define FIFO_NAME1 "my_fifo1"

int main()
{

    char *strarr[50] = {"str 01", "str 02", "str 03", "str 04", "str 05", "str 06", "str 07", "str 08", "str 09", "str 10",
                        "str 11", "str 12", "str 13", "str 14", "str 15", "str 16", "str 17", "str 18", "str 19", "str 20",
                        "str 21", "str 22", "str 23", "str 24", "str 25", "str 26", "str 27", "str 28", "str 29", "str 30",
                        "str 31", "str 32", "str 33", "str 34", "str 35", "str 36", "str 37", "str 38", "str 39", "str 40",
                        "str 41", "str 42", "str 43", "str 44", "str 45", "str 46", "str 47", "str 48", "str 49", "str 50"};

    char *intarr[50] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
                        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
                        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
                        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
                        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49"};
    mkfifo(FIFO_NAME, 0666);
    int st = 0, index = 0;
    while (st == 0)
    {
        int fifo = open(FIFO_NAME, O_WRONLY);
        printf("P1 writing data with start index: %d\n", index);

        char message[512] = "";

        int i = 0, len = strlen("str 01");
        for (i = 0; i < 5; i++)
        {
            strcat(message, strarr[i + index]);
            strcat(message, " : ");
            strcat(message, intarr[i + index]);
            strcat(message, "\n");
        }
        printf("P1 sending::\nstrinf : id\n%s\n", message);
        write(fifo, message, strlen(message));

        close(fifo);
        printf("P1 waiting for acknowledgement from P2\n\t>>><<<\n");
        sleep(3);
        fifo = open(FIFO_NAME, O_RDONLY);
        char message1[512];
        read(fifo, message1, 512);
        printf("P1 Received acknowledgement : %d\n", atoi(message1));
        index = atoi(message1);
        index++;
        if (index >= 49)
        {
            st = 1;
        }
        close(fifo);
    }
    return 0;
}