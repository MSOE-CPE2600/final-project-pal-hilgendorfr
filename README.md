# Systems Programming Lab 13 Final Project: Chat Program
## Created by: Ryan Pal Hilgendorf and Alex Toma
### Creation Process:

- The program we decided to work on for our final lab was the chat window between two terminals.
The program has the following implemented:

### chatWrite.c:
- Acts as a server which listens for incoming client connections and facilitates a two way chat
- Utilizes a UNIX domain socket for communication
- Server address structured with the socket path `/tmp/socket`
- Recieves messages from the client and prompts the user for a response
- Gracefully disconnects closing the client and reomves the socket file

### chatRead.c:
- Acts as a client which connects to a server to facilitate a two way chat
- Connects to the server using the socket path `/tmp/socket`
- Prompts the user for a message and sends to the server
- Recieves and displays responses from the server
- Closes the socket upon termination

### Build Instructions:
- type `make` to make the executables
- In the first command terminal, type `./server`
- In the second command terminal, type `./client`
- To exit the program, press `ctrl+c`

*Note: Parts of this lab were done together on Ryan's laptop as we were not both added to the repository until a few days later*
