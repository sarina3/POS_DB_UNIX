/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.h
 * Author: root
 *
 * Created on December 19, 2017, 5:26 PM
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#ifndef CLIENT_H
#define CLIENT_H
using namespace std;
class Client {
public:
    Client();
    int connection(int portNumber);
    void work();
    void createTable();
    Client(const Client& orig);
    string login();
    virtual ~Client();
private:
    int socketf,port;
    sockaddr_in server;
    string adress;
    bool checkTypesOfColums(string typ, string prikaz);
};

#endif /* CLIENT_H */

