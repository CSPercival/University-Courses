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

const int CONST_stage = 5000;
const int CONST_listening_port = 54321;
const int CONST_last_seen_lim = 5;
const u_int32_t CONST_unreachable = INT_MAX;
const u_int32_t CONST_distance_lim = 10;
struct Interfaces neighbours;
struct RoutingTable routing;