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

int main(int argc, char **argv)
{
    setup(argc, argv);

    int done = 0;
    while(!done){
        check_for_packets();
        check_for_availability();
        check_timers();
        for(int ctr = 0, buff_idx = window.read_index; ctr <  CONST_window_size, ctr++, buff_idx = (buff_idx + 1) % CONST_window_size){

        }
    }
}