//cyryl szatan 340510
#ifndef RECEIVING
#define RECEIVING

int wait_for_datagrams(int sock_fd, int *timeout);
void receive_datagrams(int sock_fd);

#endif