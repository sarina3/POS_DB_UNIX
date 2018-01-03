/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Sockets.cpp
 * Author: root
 * 
 * Created on December 24, 2017, 1:53 PM
 */

#include "Sockets.h"
#include <string>
#include <iostream>

Sockets::Sockets(int portNumber) 
{
    this->maxPacketSize = 255;
    this->socketf = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNumber);
    bind(socketf,(sockaddr *)&server_addr,sizeof(server_addr));
    listen(socketf,5);
    socklen_t clientAddrSize = sizeof(client_addr);
    socketf = accept(socketf,(sockaddr*)&client_addr,&clientAddrSize);
}
Sockets::work(){
    int n = 0;
    while(1){
        n = recv(this->socketf,msg,maxPacketSize,0);
        cout << msg;
        cout << endl;
        this->message = msg;
        if(this->message == "close"){
             send(socketf,this->message.c_str(),sizeof(this->message.c_str()),0);
             this->message = "closing connection";
             break;
        }
        this->message = "finally \n";
        send(socketf,this->message.c_str(),sizeof(this->message.c_str()),0);
}

Sockets::Sockets(const Sockets& orig) {
}

Sockets::~Sockets() {
}

