#include "utils/s2n_ecdsa_signer.h"

#define h_addr h_addr_list[0] /* for backward compatibility */

int proxy_sign(uint8_t *input, uint32_t input_length, uint8_t** output, uint32_t* output_length){
    int BUFSIZE = 1024;

    if(input_length >= BUFSIZE){
        return -1;
    }

    int sockfd, n;
    int portno = 9090;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    const char *hostname = "127.0.0.1";

    char buf[BUFSIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0){
        *output = NULL;
        *output_length = -1;

        return -1;
    }

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        *output = NULL;
        *output_length = -1;

        return -1;
    }

    /* build the server's Internet address */
    // bzero((char *) &serveraddr, sizeof(serveraddr));
    memset((char *) &serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;      
    memmove((char *)&serveraddr.sin_addr.s_addr, 
        (char *)server->h_addr, 
        server->h_length);

    serveraddr.sin_port = htons(portno);

    /* get a message from the user */
    memset(buf, 0, BUFSIZE);
    memcpy(buf, (char*)input, input_length);
    

    /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, input_length, 0, (const struct sockaddr*)&serveraddr, serverlen);
    if (n < 0){ 
        *output = NULL;
        *output_length = -1;

        return -1;
    }
    
    /* receive the response from the sign proxy */
    n = recvfrom(sockfd, buf, BUFSIZE - 1, 0, (struct sockaddr*)&serveraddr, (unsigned int*)&serverlen);
    if (n < 0){
        *output = NULL;
        *output_length = -1;

        return -1;
    }

    *output = (uint8_t*) malloc(BUFSIZE*2 + 1);
    *output_length = n;

    memcpy((char*)*output, (char*)buf, n);
    
    return 1;
}
