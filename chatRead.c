/******************************************************************************
 * Assignment: Lab 13 - chat program
 * File: chatRead.c -- client
 * Names: Ryan Pal Hilgendorf, Alexander Toma
 * Created: 12/8/2024
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFFERSIZE 256
#define SOCKETPATH "/tmp/socket"
int clientFD;

// read messages from server
void *readServer(void *arg) {
    char buffer[BUFFERSIZE];
    while (1) {
        memset(buffer, 0, BUFFERSIZE);
        int bytesRead = read(clientFD, buffer, BUFFERSIZE);
        if (bytesRead > 0) {
            printf("\nRecieved: %s\nSend: ", buffer);
            fflush(stdout);
        } else if (bytesRead == 0) {
            printf("Server Disconnected.\n");
            break;
        } else {
            perror("Error recieving messsage.");
            break;
        }
    }
    pthread_exit(NULL);
}

// send messages to server
void *writeServer(void *arg) {
    char buffer[BUFFERSIZE];
    while (1) {
        printf("Send: ");
        fgets(buffer, BUFFERSIZE, stdin);
        if (write(clientFD, buffer, strlen(buffer)) == -1) {
            perror("Error sending message.");
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    struct sockaddr_un serverAddr;

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

    // create read and write threads
    pthread_t readThread;
    pthread_t writeThread;
    pthread_create(&readThread, NULL, readServer, NULL);
    pthread_create(&writeThread, NULL, writeServer, NULL);

    // wait on threads
    pthread_join(readThread, NULL);
    pthread_cancel(writeThread);

    close(clientFD);
    return 0;
}