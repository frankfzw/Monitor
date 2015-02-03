#ifndef PARSER_H
#define PARSER_H

#include "property.h"

using namespace std;

class Parser {

private:
    /*
    file name of configure file
     */
    string fileName;

    /*
    Error tag
     */
    static const string TAG;


    /**
     * [Parser::parseConfigure private one, load the property from configure file]
     * @param  p [entity of Porperty to set]
     * @return   [0 if nothing is wrong]
     */
	int parseConfigure(struct Property &p);

    /**
     * [Parser::parseLine description]
     * @param  line  [one line of configure]
     * @param  key   [entry name of Property]
     * @param  value [entry value of Property]
     * @return       [-1 if has an syntax error]
     */
	int parseLine(string &line, string &key, string &value);

    /**
     * [Parser::setProperty description]
     * @param  p     [entity of Porperty to set]
     * @param  key   [entry name of Property]
     * @param  value [entry value of Property]
     * @return       [-1 if the key doesn't match]
     */
	int setProperty(struct Property &p, string &key, string &value);

public:
	Parser(string fileName);

    /**
     * [Parser::getProperty public one, to load property from file and set the client ip]
     * @param  p [entity of Porperty to set]
     * @return   [description]
     */
	int getProperty(struct Property &p);
};



#endif