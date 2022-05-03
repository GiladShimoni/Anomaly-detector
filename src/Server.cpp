/*
* Server.cpp
*
* Author: 208968032 Gilad Shimoni
* Author: 324234756 Marom Cohen
*/
#include "Server.h"
#include <thread>
#define NUM_EXPECTED_CLIENTS 2

/***
 * This is a constructor method which restarts instances of server
 * @param port the port to which the server listens
 */
Server::Server(int port)throw (const char*) {
    //init self address
    this->port = port;
    myAddr.sin_family = AF_INET;
    myAddr.sin_addr.s_addr = INADDR_ANY;
    myAddr.sin_port = htons(port);
    sock = socket(AF_INET, SOCK_STREAM, 0); //creat socket
    if(sock < 0 ){ throw "SOCKET ERROR";}
    int bindCheck = bind(sock, (struct sockaddr*)&myAddr, sizeof(myAddr)); //bind server address to sock
    if(bindCheck < 0){ throw "BIND ERROR";}
    int listenCheck = listen(sock, NUM_EXPECTED_CLIENTS ); //listen to port.
    if(listenCheck < 0 ){ throw "LISTEN ERROR";}
}

/***
 * This is the method that starts the server - gets the server into action
 * @param ch the client to which the server starts to listen
 */
void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this](){
        // while the connection was not terminated
        while (keepAlive) {
            socklen_t clientLen = sizeof(clientAddr);
            int clientSock = accept(sock, (struct sockaddr *) &clientAddr, &clientLen);
            if (clientSock < 0) { throw "ACCEPT ERROR"; }
            ch.handle(clientSock);
            close(clientSock);
        }
        close(sock);
    });
}

/***
 * This method stops the server from coninue working
 */
void Server::stop(){
    keepAlive = false;
    close(sock);
	t->join(); // do not delete this!

}

/***
 * This is a destructor method
 */
Server::~Server() {
}

/**
 * This method reads data the server socket had captured in the socket
 * @return the line of data read from the socket
 */
string NetworkIO::read() {
    string line = "";
    char c;
    recv(sock, &c, sizeof(char), 0);
    // reading from socket until we see '/n'
    while (c != '\n'){
        line.append(&c);
        recv(sock, &c, sizeof(char), 0);
    }
    return line;
}

/***
 * Thid method writes a specific text to the file
 * @param text the text we write to the file
 */
void NetworkIO::write(string text){
    const char* recvText = text.c_str();
    send(sock,recvText,strlen(recvText),0);
}

/***
 * the method writes the float given to the method
 * @param f number given to be written into the file
 */
void NetworkIO::write(float f) {
    ostringstream stream;
    stream << f;
    string str(stream.str());
    write(str);

}

/***
 * This method reads the data given to the method
 * @param f the number which we want to read
 */
void NetworkIO::read(float *f) {
    recv(sock,f,sizeof(float),0);
}

