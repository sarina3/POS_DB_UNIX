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
    string sendMessage(string message);
    string createTable();
    Client(const Client& orig);
    string login();
    string select();
    string insert();
    string update();
    string dropTable();
    string getTablesAll();
    string getTablesMy();
    string chmodfunc();
    string shutDown();
    string chmodreverse();
    string deleteFromTable();
    bool checkTypesOfColums(string typ, string prikaz);
    bool checDuplicatesOfPKandNotNullRows(string vstup);
    virtual ~Client();
private:
    int socketf,port;
    sockaddr_in server;
    string username;
    string adress;
};

#endif /* CLIENT_H */

