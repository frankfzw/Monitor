#ifndef UTILITY_H_
#define UTILITY_H_

#include <netinet/icmp6.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>



void printMAC(u_char *mac);

void printTCP(struct tcphdr *packet);

void printUDP(struct udphdr *udph);

void printICMP(struct icmp6_hdr *icmph);

void printETH(struct ether_header *eth);

void printIP(struct ip *iph);

#endif