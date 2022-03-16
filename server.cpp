#define	_DEFAULT_SOURCE			/* Uses POSIX network functions */
#define	INIT_BUFF_SIZE  1024			/* Initial buffer size */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


int main(int argc, char **argv){

    struct sockaddr_in server_addr;
    int server_socket;
    int port; // TODO

    // Used for receiving data
    int rcv_size;
    char *str_parser;
    char *str_buffer = (char *) malloc(sizeof(char)* INIT_BUFF_SIZE); //Used to store data

    // Used to send data
    int sent_size;

    // Initialize server information
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        perror("Error initializing server socket");
        return EXIT_FAILURE;
    }

    if(bind(server_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1){
        perror("Error binding socket");
        close(server_socket);
        return EXIT_FAILURE;
    }

    
    return 0;
}