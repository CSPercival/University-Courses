//cyryl szatan 340510

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif				// _DEFAULT_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <sys/stat.h>

#include "aux.h"

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

long long current_time_ms(){
    struct timespec ts;
    if(clock_gettime(CLOCK_REALTIME, &ts) == -1){
        ERROR("clock error");
    }
    return (ts.tv_sec * 1000LL) + (ts.tv_nsec / 1000000);
}

int string_to_int(char* ptr){
    int ans = 0;
    while((*ptr) != 0){
        ans *= 10;
        ans += (*ptr) - '0';
        ptr++;
    }
    return ans;
}

int cpy_int_to_string(char *dest, int src){
    if(src == 0){
        (*dest) = '0';
        return 1;
    }
    int src_s = (int)log10((double)src) + 1;
    dest += src_s - 1;
    while(src > 0){
        (*dest) = src % 10 + '0';
        dest--;
        src /= 10;
    }
    return src_s;
}

int check_ip_address_correctness(char *ip_address){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ip_address, &sa.sin_addr);
    return result != 0;
}

int check_port_correctness(int port){
    if(port == 0) return 0;
    if(port >= (1<<16)) return 0;
    return 1;
}

int check_path_correctness(char *path){
    struct stat stat_struct;
    if(stat(path, &stat_struct) == 0){
        if(S_ISDIR(stat_struct.st_mode)){
            //directory
            return 1;
        } else {
            //file
            return 2;
        }
    } else {
        //path is incorrect
        return 0;
    }
}

const int CONST_max_path_len = 255;
const int CONST_max_waiting_time = 1000;
const int CONST_max_packet_len = 5000000;
