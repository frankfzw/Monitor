#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>

using namespace std;

/**
 * Property of the client
 * and the cluster
 */

struct Property
{
	/* data from configure.txt*/
	string myName;
	string masterIP; 
    string deviceName;

	/*
	data from eth
	 */
	string myIP;
};

#endif