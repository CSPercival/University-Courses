//cyryl szatan 340510
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif				// _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "aux_func.h"
#include "aux_structs.h"
#include "sending.h"

u_int16_t compute_icmp_checksum(const void *buff, int length){
    const u_int16_t* ptr = buff;
    u_int32_t sum = 0;
    assert (length % 2 == 0);
    for (; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16U) + (sum & 0xffffU);
    return ~(sum + (sum >> 16U));
}

void send_icmp_request(int sock_fd, char *dest_ip, int ttl, struct icmp_reply_status *replies){
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = htons(getpid() & 0xFFFF);

    struct sockaddr_in recipient;
    memset(&recipient, 0, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, dest_ip, &recipient.sin_addr);

    if(setsockopt(sock_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) == -1){
        ERROR("failed to set socket option");
    }

    for(int i = 0; i < 3; i++){
        header.icmp_hun.ih_idseq.icd_seq = htons(ttl * 3 + i);
        header.icmp_cksum = 0;
        header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));
        replies->sending_time[i] = current_time_ms();
        ssize_t bytes_sent = sendto(sock_fd, &header, sizeof(header), 0, (struct sockaddr*)&recipient, sizeof(recipient));
        if(bytes_sent == -1){
            ERROR("Failed to send icmp request");
        }
    }
}
