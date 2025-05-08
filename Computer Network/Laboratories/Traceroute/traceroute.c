//cyryl szatan 340510
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <assert.h>

#include "aux_func.h"
#include "aux_structs.h"
#include "sending.h"
#include "receiving.h"

int duplicate_ip(char *ip1, char *ip2){
    while((*ip1) == (*ip2) && (*ip1) != 0){
        ip1++;
        ip2++;
    }
    if((*ip1) == 0 && (*ip2) == 0) return 1;
    return 0;
}

void print_reply(struct icmp_reply_status *replies, int ttl){
    printf("%d. ", ttl);
    if(replies->reply_ctr == 0){
        printf("*\n");
        return;
    }

    int unique = 1;
    int time_mean = 0;
    for(int i = 0; i < replies->reply_ctr; i++){
        time_mean += replies->time[i];
        unique = 1;
        for(int j = 0; j < i; j++){
            if(duplicate_ip(replies->ipaddr[i], replies->ipaddr[j])){
                unique = 0;
                break;
            }
        }
        if(unique){
            printf("%s ",replies->ipaddr[i]);
        }
    }

    if(replies->reply_ctr < 3){  
        printf("???\n");
    } else {
        time_mean /= 3;
        printf("%dms\n",time_mean);
    }
    return;
}

int main(int argc, char **argv)
{
    char *dest_ip;
    int sock_fd;
    
    if(argc != 2){
        ERROR("invalid arguments");
    }
    dest_ip = argv[1];
    if(!check_ip_address_correctness(dest_ip)){
        ERROR("invalid ip address");
    }

    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock_fd < 0){
        ERROR("socket error");
    }

    for(int ttl_ctr = 1; ttl_ctr <= 30; ttl_ctr++){
        struct icmp_reply_status replies;
        replies.reply_ctr = 0;
        replies.final_reply = 0;

        send_icmp_request(sock_fd, dest_ip, ttl_ctr, &replies);
        
        receive_icmp_reply(sock_fd, ttl_ctr, &replies);
        
        print_reply(&replies, ttl_ctr);
        
        if(replies.final_reply) break;
    }
}
