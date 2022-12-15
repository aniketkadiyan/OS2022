// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // socket setup done
    //  write data now

    char *strarr[50] = {"00 : str 01", "01 : str 02", "02 : str 03", "03 : str 04", "04 : str 05", "05 : str 06", "06 : str 07", "07 : str 08", "08 : str 09", "09 : str 10",
                        "10 : str 11", "11 : str 12", "12 : str 13", "13 : str 14", "14 : str 15", "15 : str 16", "16 : str 17", "17 : str 18", "18 : str 19", "19 : str 20",
                        "20 : str 21", "21 : str 22", "22 : str 23", "23 : str 24", "24 : str 25", "25 : str 26", "26 : str 27", "27 : str 28", "28 : str 29", "29 : str 30",
                        "30 : str 31", "31 : str 32", "32 : str 33", "33 : str 34", "34 : str 35", "35 : str 36", "36 : str 37", "37 : str 38", "38 : str 39", "39 : str 40",
                        "40 : str 41", "41 : str 42", "42 : str 43", "43 : str 44", "44 : str 45", "45 : str 46", "46 : str 47", "47 : str 48", "48 : str 49", "49 : str 50"};

    int i = 0, st = 1,index=0;
    while (st == 1)
    {
        for (i = 0; i < 5; i++)
        {
            printf("\t\tclient sending data with id : %d\n",i+index);
            send(sock, strarr[index+i], strlen(strarr[index+i]), 0);
        }

        // send(sock, hello, strlen(hello), 0);

        // printf("Hello message sent\n");

        valread = read(sock, buffer, 2);
        index = atoi(buffer);
        printf("\n\t\tclient acknowledged : %d\n", index);
        index++;
        if(index>=49){
            st=0;
        }
        sleep(3);
    }

    // closing the connected socket
    printf("\n\t\tclient shutdown\n");
    close(client_fd);
    return 0;
}
