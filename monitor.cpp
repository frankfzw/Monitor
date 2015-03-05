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
#include <pthread.h>
#include <unistd.h>
#include <fstream>


using namespace std;

void *showWorkingGadget(void *args) {
    
    int i = 0;
    string out = "\b|";
    cout<<"Working....\t"<<out;
    
    while(true) {
        usleep(100);
        i = (i + 1) % 5;
        switch(i) {
        case 0:
            out = "\b|";
            break;
        case 1:
            out = "\b\\";
            break;
        case 2:
            out = "\b-";
            break;
        case 3:
            out = "\b/";
            break;
        case 4:
            out = "\b-";
            break;
        default:
            out = "\b|";
        }
        cout<<out;
    }
    
    return NULL;
}

void filter(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
	//TODO
	//fill this filter
    //get property
    struct Property *p = (struct Property *)args;

    //only capture ip
	struct ether_header *ethernet;  /* The ethernet header [1] */
	ethernet = (struct ether_header*)(packet);
    if (ntohs(ethernet->ether_type) != ETHERTYPE_IP)
        return;
    //printETH(ethernet);
    struct ip *iph = (struct ip*)(packet + SIZE_ETHERNET);
    //printIP(iph);
    string srcIP(inet_ntoa(iph->ip_src));
    string dstIP(inet_ntoa(iph->ip_dst));
    if ((p->in.find(srcIP) != p->in.end()) && (dstIP.compare(p->myIP) == 0)) {
        p->in[srcIP] ++;
        //cout<<"From "<<srcIP<<": "<<p->from[srcIP]<<endl;
    }
    else if ((p->out.find(dstIP) != p->out.end()) && (srcIP.compare(p->myIP) == 0)) {
        p->out[dstIP] ++;
        //cout<<"To "<<dstIP<<": "<<p->to[dstIP]<<endl;
    }
    else {
        //ingore the packet
        return;
    }
    

}

void *writeData(void *args) {
    //cout<<"Working...   |";
    struct Property *p = (struct Property *)args;
    //create log file and write the number of clients in cluster with count interval
    ofstream ofs;
    ofs.open("in", fstream::out);
    ofs<<p->in.size()<<endl<<INTERVAL<<endl;
    ofs.close();
    ofs.open("out", fstream::out);
    ofs<<p->out.size()<<endl<<INTERVAL<<endl;
    ofs.close();
    while (true) {
        usleep(INTERVAL);
        ofs.open("in", fstream::out | fstream::app);
        //cout<<"Writing Data:\n";
        map<string, int>::iterator it;
        //cout<<"From:\n";
        for (it = p->in.begin(); it != p->in.end(); it++) {
            ofs<<it->first<<"\t"<<it->second<<endl;
        }
        ofs.close();
        ofs.open("out", fstream::out | fstream::app);
        //cout<<"To:\n";
        for (it = p->out.begin(); it != p->out.end(); it++) {
            ofs<<it->first<<"\t"<<it->second<<endl;
        }
        ofs.close();
    }
    return NULL;
}

int main (int arg, char *argv[]) {
	struct Property p;
	Parser parser("configure");
	parser.getProperty(p);

	cout<<p.myName<<endl
		<<p.masterIP<<endl
		<<p.deviceName<<endl
		<<p.myIP<<endl;

	cout<<"Cluster:\n";
	map<string, int>::iterator it;
	for (it = p.in.begin(); it != p.in.end(); it++) {
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
    //create a thread to show the working status
    pthread_t showThread;
    int sr;
    sr = pthread_create(&showThread, NULL, showWorkingGadget, NULL);
    if (sr) {
        cout<<"Pthread Error!!\n";
        exit(EXIT_FAILURE);
    }
    //create a thread to write the data into file per 100ms
    pthread_t thread;
    int r;
    r = pthread_create(&thread, NULL, writeData, (void *)&p);
    if (r) {
        cout<<"Pthread Error!!\n";
        exit(EXIT_FAILURE);
    }
	pcap_loop(handle, num_pkts, filter, (u_char *)&p);

}