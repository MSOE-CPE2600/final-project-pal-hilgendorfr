/******************************************************************************
 * Assignment: Lab 13 - chat program
 * File: chatWrite.c
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
    int serverFD;
    int clientFD;
    struct sockaddr_un serverAddr;
    char buffer[BUFFERSIZE];

    // create UNIX domain socket
    if ((serverFD = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failure");
        exit(EXIT_FAILURE);
    }

    // set up server address structure
    memset(&serverAddr, 0, sizeof(struct sockaddr_un));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SOCKETPATH, sizeof(serverAddr.sun_path) - 1);

    // remove socket file if present
    unlink(SOCKETPATH);

    // bind socket to path
    if (bind(serverFD, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_un)) == -1) {
        perror("Binding Error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    // listen for incoming connection
    if (listen(serverFD, 5) == -1) {
        perror("Listening error");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on path %s\n", SOCKETPATH);
    
    // accept client connection
    if ((clientFD = accept(serverFD, NULL, NULL)) == -1) {
        perror("accept");
        close(serverFD);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the client.\n");

    while(1) {
        // read data from client
        memset(buffer, 0, BUFFERSIZE);
        int bytesRead = read(clientFD, buffer, BUFFERSIZE);
        if (bytesRead > 0) {
            printf("Recieved: %s\n", buffer);
        } else if (bytesRead == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            perror("Error recieving message.");
            break;
        }

        // get server response
        printf("Send: ");
        fgets(buffer, BUFFERSIZE, stdin);

        // send response to client
        if (write(clientFD, buffer, strlen(buffer)) == -1) {
            perror("Error sending message.");
            break;
        }
    }

    close(clientFD);
    close(serverFD);
    unlink(SOCKETPATH);
    return 0;
}