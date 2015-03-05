#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>
#include <netinet/in.h>
#include <map>

using namespace std;

/**
 * Property of the client
 * and the cluster
 */

struct Property
{
	/* data from configure file*/
	string myName;
	string masterIP; 
    string deviceName;
    map<string, int>in;
    map<string, int>out;

	/*
	data from eth
	 */
	string myIP;
};

#endif