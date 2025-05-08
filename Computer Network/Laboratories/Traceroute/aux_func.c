//cyryl szatan 340510
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif				// _DEFAULT_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include "aux_func.h"

void ERROR(const char* str){
    if(errno == 0){
        fprintf(stderr, "%s\n", str);
    } else {
        fprintf(stderr, "%s: %s\n", str, strerror(errno));  // NOLINT(*-err33-c)
    }
    exit(EXIT_FAILURE);
}


void print_as_bytes (unsigned char* buff, ssize_t length){
    for (ssize_t i = 0; i < length; i++, buff++)
        printf("%.2x ", *buff);
}

int check_ip_address_correctness(char *ip_address){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address, &sa.sin_addr);
    return result != 0;
}

long long current_time_ms(){
    struct timespec ts;
    if(clock_gettime(CLOCK_REALTIME, &ts) == -1){
        ERROR("clock error");
    }
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000);
}