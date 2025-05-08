//cyryl szatan 340510
#ifndef RECEIVING
#define RECEIVING

#include "aux_structs.h"

void receive_icmp_reply(int sock_fd, int ttl, struct icmp_reply_status *replies);

#endif
