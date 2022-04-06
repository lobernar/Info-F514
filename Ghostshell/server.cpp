#define	_DEFAULT_SOURCE			/* Uses POSIX network functions */
#define	INIT_BUFF_SIZE  1024			/* Initial buffer size */
#define BACKLOG 20
#include <stdlib.h>
#include <iostream>
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
    int port = 8080; // TODO

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

    if(listen(server_socket, BACKLOG) == -1){
        perror("Error while listening on socket");
        close(server_socket);
        return EXIT_FAILURE;
    }


    /*
    ---------------------------------------------------------
        Accepting connections
    ---------------------------------------------------------
    */
    while(1){
        struct sockaddr_in client_address;
        int client_socket, str_len;
        str_len = 5;
        socklen_t client_addr_len;
        char* msg;

        //Used to send the length of a string
        uint32_t pack_len;

        bzero(&client_address, sizeof(struct sockaddr_in));

        client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len);
        if(client_socket == -1){
            perror("Unable to accept client connection\n");
            continue;
        }

        for(str_parser = msg; int receive_size = 0; receive_size < str_len){
            int ret = recv(client_socket, str_parser, str_len - receive_size, 0);
            //Check for errors
            if(ret == -1){
                perror("Unable to receive message\n");
                close(client_socket);
                break;
            }else if(ret == 0){
                perror("Client socket closed\n");
                break;
            }

            receive_size += ret;
            str_parser += ret;
        }
        std::cout << msg << std::endl;

    }   
    close(server_socket); 
    return 0;
}