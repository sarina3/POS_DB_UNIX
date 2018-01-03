/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sockets.h
 * Author: root
 *
 * Created on December 24, 2017, 1:53 PM
 */

#ifndef SOCKETS_H
#define SOCKETS_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex>
#include <string>

using namespace std;
class Sockets {
public:
    Sockets();
    void work();
    Sockets(const Sockets& orig);
    virtual ~Sockets();
private:
    int socketf,maxPacketSize;
    sockaddr_in  server_addr,client_addr;
    string message;
    char msg[255];
};

#endif /* SOCKETS_H */

