/*
* Author: demees@ 
* Summary: ECDSA using signer proxy
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int proxy_sign(uint8_t *input, uint32_t input_length, uint8_t** output, uint32_t *output_length);
