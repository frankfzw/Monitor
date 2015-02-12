#include "monitor.h"
#include "property.h"
#include "parser.h"
#include "utility.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <iostream>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ethernet.h>

#define SNAP_LEN 1518

using namespace std;

void filter(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
	//TODO
	//fill this filter
	struct ether_header *ethernet;  /* The ethernet header [1] */
	ethernet = (struct ether_header*)(packet);

	printETH(ethernet);

}

int main (int arg, char *argv[]) {
	struct Property p;
	Parser parser("configure");
	parser.getProperty(p);

	cout<<p.myName<<endl
		<<p.masterIP<<endl
		<<p.deviceName<<endl
		<<p.myIP<<endl;

	map<string, int>::iterator it;
	for (it = p.from.begin(); it != p.from.end(); it++) {
		cout<<it->first<<"\t"<<it->second<<endl;
	}

	//open device and listen
	//bpf_u_int32 mask;
	//bpf_u_int32 net;
	pcap_t *handle;
	int num_pkts = -1;

	//do some check here
	char errbuf[PCAP_ERRBUF_SIZE];
	//open device
	handle = pcap_open_live(p.deviceName.c_str(), SNAP_LEN, 1, 1000, errbuf);
    if (handle == NULL)
    {
        cout<<"Cannot open: "<<p.deviceName<<endl;
		exit(EXIT_FAILURE);
    }

	if (pcap_datalink(handle) != DLT_EN10MB) {
		cout<<p.deviceName<<" is not an Ethernet device\n";
		exit(EXIT_FAILURE);
	}

	//set direction, only capture packets that are sent out of the nic
	/*
	if (pcap_setdirection(handle, PCAP_D_OUT) == -1) {
		cout<<p.deviceName<<" set direction failed\n";
		exit(EXIT_FAILURE);
	}
	*/

	//set filter, only capture ip packet
	struct in_addr ipAddr;
	struct bpf_program fp;
	inet_pton(AF_INET, p.myIP.c_str(), &ipAddr);
	if (pcap_compile(handle, &fp, "ip", 0, (bpf_u_int32)ipAddr.s_addr) == -1) {
		cout<<p.deviceName<<" set set filter failed\n";
		exit(EXIT_FAILURE);
	}
	if (pcap_setfilter(handle, &fp) == -1) {
		cout<<p.deviceName<<" couldn't install filter\n";
		exit(EXIT_FAILURE);
	}

	cout<<p.deviceName<<" is ready\n";
	pcap_loop(handle, num_pkts, filter, NULL);

}