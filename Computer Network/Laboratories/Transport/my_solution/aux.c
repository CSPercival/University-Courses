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

int string_to_int(char* ptr){
    int ans = 0;
    while((*ptr) != 0){
        ans *= 10;
        ans += (*ptr) - '0';
        ptr++;
    }
    return ans;
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

void timer_set(struct Timer *t, int timeout){
    t->timeout = timeout;
    t->time_left = timeout;
    t->running = 0;
}
void timer_start(struct Timer *t){
    t->running = 1;
    t->start_time = current_time_ms();
}
void timer_stop(struct Timer *t){
    t->end_time = current_time_ms();
    if(!t->running) ERROR("Timer error - tried to stop timer that is not running");

    t->running = 0;
    t->elapsed_time = t->end_time - t->start_time;
    t->time_left -= t->elapsed_time;
}
void timer_reset(struct Timer *t){
    t->time_left = t->timeout;
}
int timer_get(struct Timer *t){
    return t->time_left;
}

char* dest_ip;
int dest_port;
char* out_file_name;
FILE *out_file;
int data_length;

int sock_fd_send;
int sock_fd_list;

struct CyclicBuffer window;

const int CONST_window_size = 10;
const int CONST_data_size = 1000;
const int CONST_datagram_size = 1050;
const int CONST_timeout = 100;

// const int CONST_stage = 5000;
// const int CONST_listening_port = 54321;
// const int CONST_last_seen_lim = 5;
// const u_int32_t CONST_unreachable = INT_MAX;
// const u_int32_t CONST_distance_lim = 10;
// struct Interfaces neighbours;
// struct RoutingTable routing;