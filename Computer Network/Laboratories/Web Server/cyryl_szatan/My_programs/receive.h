//cyryl szatan 340510
#ifndef RECEIVING
#define RECEIVING

#include "aux.h"

void parse_packet(struct RequestInformations *req_info);
int receive_packets(int sock_fd, struct RequestInformations *req_info);
void wait_for_clients(int sock_fd_main, int *sock_fd_connect);

#endif