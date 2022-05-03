/*
* CLI.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#ifndef CLI_H_
#define CLI_H_


#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    CliRun* cliRun{};
	vector<Command*> commands;

public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
