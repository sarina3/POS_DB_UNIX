/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.h
 * Author: root
 *
 * Created on December 19, 2017, 5:25 PM
 */
#ifndef SERVER_H
#define SERVER_H
#include "SharedData.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex>
#include <string>
#include "Database.h"
#include <iostream>
#include <pthread.h>
using namespace std;
class server {
public:
    server(int portNumber);
    server(const server& orig);
    static void* work(void* pdata);
    static void* listening(void* pdata);
    void* something();
    virtual ~server();
private:
    int socketf,maxPacketSize;
    sockaddr_in  server_addr,client_addr;
    string message;
    Database *database;
    char msg[255];
    SharedData *data;
    
};

#endif /* SERVER_H */

