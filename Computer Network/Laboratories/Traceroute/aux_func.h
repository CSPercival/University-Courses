//cyryl szatan 340510
#ifndef AUX_FUNCTIONS
#define AUX_FUNCTIONS

#include <unistd.h>

void ERROR(const char* str);
void print_as_bytes (unsigned char* buff, ssize_t length);
int check_ip_address_correctness(char *ip_address);
long long current_time_ms();

#endif