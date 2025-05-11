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

#include "aux.h"
#include "setup.h"
#include "receive.h"
#include "send.h"
#include "application.h"

int main(int argc, char **argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    // printf("Setup start\n");
    setup(argc, argv);
    // printf("Setup done\n");
    // printf("ip: %s\n", GLOBAL_dest_ip);

    int bytes_analyzed = 0;
    int ready;
    // int new_analyzed_bytes = 1;
    while(bytes_analyzed < GLOBAL_data_length){
        // if(new_analyzed_bytes)
            // printf("Progres : %d\n", bytes_analyzed);
        // printf("Wait for datagrams\n");
        ready = wait_for_datagrams(window.dp[queue_front(&timeout_queue).idx]->timer.time_left);
        // printf("End pf Waiting  ready = %d\n", ready);
        if(ready != 0){
            // printf("Datagrams received\n");
            receive_datagrams();
            // printf("Datagrams parsed\n");
        }
        // printf("Pass to App\n");
        // new_analyzed_bytes = pass_data_to_app();
        // bytes_analyzed += new_analyzed_bytes;
        bytes_analyzed += pass_data_to_app(bytes_analyzed);
        // printf("App got data\n");
        check_timers();
        // printf("All datagrams send\n");
    }
    printf("DONE\n");
}