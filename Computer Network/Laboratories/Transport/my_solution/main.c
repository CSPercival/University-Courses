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
    setup(argc, argv);

    int bytes_analyzed = 0;
    int ready;
    while(bytes_analyzed < GLOBAL_data_length){
        ready = wait_for_datagrams(window.dp[0]->timer.time_left);
        if(ready != 0){
            receive_datagrams();
        }
        bytes_analyzed += past_data_to_app();
        check_timers();
    }
}