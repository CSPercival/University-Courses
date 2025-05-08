//cyryl szatan 340510
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "setup.h"
#include "receiving.h"
#include "sending.h"
#include "maintenance.h"
#include "aux.h"


int main(){

    int lis_sock_fd, sen_sock_fd;
    setup_router(&lis_sock_fd, &sen_sock_fd);

    int timeout = CONST_stage;
    int ready;
    int ctr = 1;
    while(1){
        ready = wait_for_datagrams(lis_sock_fd, &timeout);
        if(ready != 0){
            receive_datagrams(lis_sock_fd);
        }

        if(timeout > 0) continue;
        spread_routing_table(sen_sock_fd);
        inspect_last_seen();
        timeout = CONST_stage;
        printf("TURA %d",ctr);
        ctr += 1;
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
        print_routing_table();
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
    }

    return 0;
}
