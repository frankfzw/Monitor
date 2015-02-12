#include "utility.h"
#include <arpa/inet.h>
#include <stdio.h>

void printMAC(u_char *mac)
{
    printf("MAC: %x:%x:%x:%x:%x:%x", 
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void printTCP(struct tcphdr *packet)
{
    printf("TCP Header:\n");
    printf("\tSource port: 0x%x\n", ntohs(packet->th_sport));
    printf("\tDestination prot: 0x%x\n", ntohs(packet->th_dport));
    printf("\tSeq Number: 0x%x\n", ntohs(packet->th_seq));
    printf("\tAck Number: 0x%x\n", ntohs(packet->th_ack));
    //printf("\tData offset: 0x%x\n", TH_OFF(packet));
    printf("\tWindow size: 0x%x\n", ntohs(packet->th_win));
    printf("\tChecksum: 0x%x\n", ntohs(packet->th_sum));
}

void printUDP(struct udphdr* udph)
{
    printf("UDP Header:\n");
    printf("\tSource port: 0x%x\n", ntohs(udph->source));
    printf("\tDestination port: 0x%x\n", ntohs(udph->dest));
    printf("\tLength: 0x%x\n", ntohs(udph->len));
    printf("\tChecksum: 0x%x\n", ntohs(udph->check));
}

void printICMP(struct icmp6_hdr *icmph)
{
    printf("ICMP Header:\n");
    printf("\tType: 0x%x\n", ntohs(icmph->icmp6_type));
    printf("\tCode: 0x%x\n", ntohs(icmph->icmp6_code));
    printf("\tChecksum: 0x%x\n", ntohs(icmph->icmp6_cksum));
}

void printETH(struct ether_header *eth)
{
    printf("Ethernet Header:\n");
    printf("\tFrom: ");
    printMAC(eth->ether_shost);
    printf("\n\tTo: ");
    printMAC(eth->ether_dhost);
    printf("\n");
}

void printIP(struct ip *iph)
{
    printf("IP Header:\n");
    //printf("\tHeader Length: 0x%x\n", IP_HL(iph));
    //printf("\tVersion: 0x%x\n", IP_V(iph));
    printf("\tService: 0x%x\n", iph->ip_tos);
    printf("\tTotal Length: 0x%x", ntohs(iph->ip_len));
    printf("\tID: 0x%x\n", ntohs(iph->ip_id));
    printf("\tFragment Flag: 0x%x\n", ntohs(iph->ip_off));
    printf("\tChecksum: 0x%x\n", ntohs(iph->ip_sum));
    printf("\tTTL: 0x%x\n", iph->ip_ttl);
    printf("\tFrom: %s\n", inet_ntoa(iph->ip_src));
    printf("\tTo: %s\n", inet_ntoa(iph->ip_dst));
}