//cyryl szatan 340510
#ifndef SENDING
#define SENDING

#include "aux_structs.h"

void send_icmp_request(int sock_fd, char *dest_ip, int ttl, struct icmp_reply_status *replies);

#endif