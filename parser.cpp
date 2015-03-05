#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

using namespace std;

const string Parser::TAG = "PARSER";

Parser::Parser(string fileName) {
	this->fileName = fileName;
}

/**
 * [Parser::parseLine description]
 * @param  line  [one line of configure]
 * @param  key   [entry name of Property]
 * @param  value [entry value of Property]
 * @return       [-1 if has an syntax error]
 */
int Parser::parseLine(string &line, string &key, string &value) {
    //cout<<TAG<<"\t"<<line;
    if (line.size() == 0)
        return 0;
    
	int pos = line.find(":");
	if (pos == string::npos) {
		return -1;
	}

	key = line.substr(0, pos);
	value = line.substr(pos + 1);
	return 0;
}

/**
 * [Parser::setProperty description]
 * @param  p     [entity of Porperty to set]
 * @param  key   [entry name of Property]
 * @param  value [entry value of Property]
 * @return       [-1 if the key doesn't match]
 */
int Parser::setProperty(struct Property &p, string &key, string &value) {
	if (key.compare("myName") == 0) {
		p.myName = value;
	}
    else if (key.compare("masterIP") == 0) {
        p.masterIP = value;
    }
    else if (key.compare("deviceName") == 0) {
        p.deviceName = value;
    }
    else if (key.compare("cluster") == 0) {
        //set entry of maps
        string::size_type sz;
        int lineNum = stoi(value, &sz); 
        return lineNum;
    }
    else if (key.compare("_cluster") == 0) {
        //struct sockaddr_in addr;
        //inet_pton(AF_INET, value.c_str(), &(addr.sin_addr));
        if (value.compare(p.myIP) == 0)
            return 0;

        p.in[value] = 0;
        p.out[value] = 0;
    }
    else if (key.compare("myIP") == 0) {
        p.myIP = value;
    }
    else
        return -1;

    return 0;
}

/**
 * [Parser::parseConfigure private one, load the property from configure file]
 * @param  p [entity of Porperty to set]
 * @return   [0 if nothing is wrong]
 */
int Parser::parseConfigure(struct Property &p) {
	ifstream ifs;
	ifs.open(fileName.c_str(), ifstream::in);
	string line;

	//get name of client
	getline(ifs, line);
	while (ifs.good()) {
        string key = "";
        string value = "";
        int res = parseLine(line, key, value);
        if (res < 0) {
            cout<<TAG<<"\t"<<"Configure file Error !\n";
            exit(EXIT_FAILURE);
        }
        res = setProperty(p, key, value);
        if (res < 0) {
            cout<<TAG<<"\t"<<"Property name Error !\n";
            exit(EXIT_FAILURE);
        }
        else if (res > 0) {
            //parse ip of cluster
            while (res > 0) {
                res--;
                getline(ifs, line);
                string temp("_cluster");
                setProperty(p, temp, line);
            }
        }
        getline(ifs, line);
    }

    ifs.close();

    return 0;
}

/**
 * [Parser::getProperty public one, to load property from file and set the client ip]
 * @param  p [entity of Porperty to set]
 * @return   [description]
 */
int Parser::getProperty(struct Property &p) {

    /*
    get ip from eth by libpcap
     */
    /*
    const char *nicName = p.deviceName.c_str();
    //open corresponding nic and get IP
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, nicName, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);

    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr; 
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr->sin_addr), str, INET_ADDRSTRLEN);
    string ipStr(str);
    p.myIP = ipStr;
    */
   
    /*
    get property from file
     */
    parseConfigure(p);

    return 0;
    
}