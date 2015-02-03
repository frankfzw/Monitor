#include "monitor.h"
#include "property.h"
#include "parser.h"

#include <iostream>

using namespace std;

int main (int arg, char *argv[]) {
	struct Property p;
	Parser parser("configure");
	parser.getProperty(p);

	cout<<p.myName<<endl
		<<p.masterIP<<endl
		<<p.deviceName<<endl
		<<p.myIP<<endl;
}