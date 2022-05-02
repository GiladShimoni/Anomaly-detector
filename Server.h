/*
* Server.h
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <sys/socket.h>  // The header file socket.h includes a number of definitions of structures needed for sockets.
#include <netinet/in.h>  // The header file in.h contains constants and structures needed for internet domain addresses.

#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <sstream>


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

// you can add helper classes here and implement on the cpp file

class NetworkIO: public DefaultIO{
    int sock;
public:
    NetworkIO(int sock):sock(sock){}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        NetworkIO io(clientID);
        CLI cli(&io);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    sockaddr_in myAddr;
    sockaddr_in clientAddr;
    int port;
    int sock;
    condition_variable cv;
    mutex m;
	thread* t; // the thread to run the start() method in
    bool keepAlive = true;

	// you may add data members

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
    void portListen(ClientHandler& ch,Server* server)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
