#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
// Ghostshell
#include <helib/helib.h>
#include <helib/EncryptedArray.h>
#include <NTL/ZZX.h>
#include <NTL/lzz_pXFactoring.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
// Client-Server
#define	_DEFAULT_SOURCE			/* Uses POSIX network functions */
#define	INIT_BUFF_SIZE  1024			/* Initial buffer size */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

using namespace helib;

int main(int argc, char **argv){

    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    /*
    // TODO: INIT PORT
    int port; 
    char *ip;

    int local_socket;

    // Used for sending messages
    uint32_t packet_size;
    int string_size;
    char *str_parser;
    int sent_size = 0;

    // Used for receiving messages
    char *str_buffer;
    int rcv_size = 0;
    int curr_size = 0;

    // Structur initialization
    bzero(&client_addr, sizeof(struct sockaddr_in));
    bzero(&server_addr, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_aton(ip, &server_addr.sin_addr) == 0){
        perror("Could not convert IP address");
        return EXIT_FAILURE;
    }

    // Client socket initialization
    local_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(local_socket == -1){
        perror("Error while initializing socket");
        return EXIT_FAILURE;
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0); // OS chooses port
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(local_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1){
        perror("Error while binding client socket with address");
        close(local_socket);
        return EXIT_FAILURE;
    }
    */

    /******************************************************************
     * GHOSTSHELL SETUP
    *******************"***********************************************/

    int lambda = 80;  // Security parametetr
    int L = 5;  // Multiplicative depth
    long p = 2;  // Prime for plaintext base
    int r = 15; // p^r is the plaintext space (represented by M in the paper (Z_M))
    long N[] = {330, 630, 682, 1285};  // Number of plaintext slots
    long m[] = {10261, 8191, 15709, 43691}; // Degree of cyclotomic polynomial

    // Build private key
    Context context();

    return 0;
}