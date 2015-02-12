#ifndef UTILITY_H_
#define UTILITY_H_

#include <netinet/icmp6.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

void printMAC(u_char *mac);

void printTCP(struct sniff_tcp *packet);

void printUDP(struct udphdr *udph);

void printICMP(struct icmp6_hdr *icmph);

void printETH(struct ether_header *eth);

void printIP(struct sniff_ip *iph);

#endif