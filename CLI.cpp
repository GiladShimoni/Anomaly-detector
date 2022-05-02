/*
* CLI.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#include "CLI.h"
#include <array>

/***
 * create commands and cliRun
 * @param dio the defaultIO given as a parameter to the instance
 */
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->cliRun = new CliRun();
    commands.push_back(new EnterTs(dio, cliRun));
    commands.push_back(new Settings(dio, cliRun));
    commands.push_back(new DetectAnomalies(dio, cliRun));
    commands.push_back(new DisplayResults(dio, cliRun));
    commands.push_back(new CompareResults(dio, cliRun));
    commands.push_back(new Exit(dio, cliRun));
}

/***
 * This method starts the CLI
 */
void CLI::start(){
    //print menu
    while (cliRun->keepAlive){
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        int i = 1;
        for(Command* c: commands){
            dio->write(to_string(i) +"."+ c->getDescription()+"\n");
            i++;
        }
        int inp = stoi(dio->read());
        commands[inp - 1]->execute();   //execute command
    }
}

/**
 * This is a destructor method
 */
CLI::~CLI() {
    for(Command* c: commands){
        delete c;
    }
    commands.clear();
    delete cliRun;
}


