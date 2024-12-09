/******************************************************************************
 * Assignment: Lab 13 - chat program
 * File: chatRead.c
 * Names: Ryan Pal Hilgendorf, Alexander Toma
 * Created: 12/8/2024
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFFERSIZE 256
#define SOCKETPATH "/tmp/socket"

int main() {
    int clientFD;
    struct sockaddr_un serverAddr;
    char buffer[BUFFERSIZE];

    // create UNIX domain socket
    if ((clientFD = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failure");
        exit(EXIT_FAILURE);
    }

    // set up server address structure
    memset(&serverAddr, 0, sizeof(struct sockaddr_un));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SOCKETPATH, sizeof(serverAddr.sun_path) - 1);

    // connect server
    if (connect(clientFD, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_un)) == -1) {
        perror("Connection Error");
        close(clientFD);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    while (1) {
        // get message from client
        printf("Send: ");
        fgets(buffer, BUFFERSIZE, stdin);

        // send message to server
        if (write(clientFD, buffer, strlen(buffer)) == -1) {
            perror("Error sending message.");
            break;
        }

        // read response from server
        memset(buffer, 0, BUFFERSIZE);
        int bytesRead = read(clientFD, buffer, BUFFERSIZE);
        if (bytesRead > 0) {
            printf("Recieved: %s\n", buffer);
        } else if (bytesRead == 0) {
            printf("Server Disconnected.\n");
            break;
        } else {
            perror("Error recieving messsage.");
            break;
        }
    }
    close(clientFD);
    return 0;
}