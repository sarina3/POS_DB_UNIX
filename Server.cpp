/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   server.cpp
 * Author: root
 * 
 * Created on December 19, 2017, 5:25 PM
 */

#include <string>

#include "Server.h"
#include <iostream>

server::server(int portNumber) {

    this->data = new SharedData();
    this->maxPacketSize = 255;
    this->socketf = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNumber);
    
    bind(socketf,(sockaddr *)&server_addr,sizeof(server_addr));
    this->data->activeSockets->push_back(socketf);
    pthread_t *thread;
    void* (*listeening)(void*) = listening;
    pthread_create(thread,NULL,listeening,data);
    data->threads->push_back(thread);
    
    
}

void* server::something() {
    while(1){};
}

void* server::listening(void* pdata){
    SharedData *data = (SharedData*)(pdata);
    int socketf,newsocket;
    
    if(data ->activeSockets->size() > 0){
        pthread_mutex_lock(&data->mutexSockets);
        socketf = data->activeSockets->at(data->activeSockets->size() - 1);
        pthread_mutex_unlock(&data->mutexSockets);
    }else{
        return 0;
    }
    sockaddr_in  server_addr,client_addr;
    while(1){
    listen(socketf,5);
    socklen_t clientAddrSize = sizeof(client_addr);
    newsocket = accept(socketf,(sockaddr*)&client_addr,&clientAddrSize);
   
    pthread_t thread;
    void* (*worrk) (void*) = work;
    cout << socketf << newsocket << endl;
    pthread_create(&thread,NULL,worrk,data);
    pthread_mutex_lock(&data->mutexSockets);
    data->activeSockets->push_back(newsocket);
    pthread_mutex_unlock(&data->mutexSockets);
    pthread_mutex_lock(&data->mutexThreads);
    data->threads->push_back(&thread);
    pthread_mutex_unlock(&data->mutexThreads);   
    
    }
    data = nullptr;
    pthread_exit(NULL);
}
void* server::work(void* pdata){
    //cout << "server alive\n";
    SharedData * data = (SharedData*)pdata;
    string message;
    int maxPacketSize = 255;
    int socketf;
    if(data ->activeSockets->size() > 0){
        pthread_mutex_lock(&data->mutexSockets);
        socketf = data->activeSockets->at(data->activeSockets->size() - 1);
        pthread_mutex_unlock(&data->mutexSockets);
    }else{
        return 0;
    }

    char msg[255];
    int n = 0;  
    while(1){
        n = recv(socketf,msg,maxPacketSize,0);
        cout << msg;
        cout << endl;
        message = msg;
        if(message == "close"){
            message = "closing connection";
            send(socketf,message.c_str(),maxPacketSize,0);
             
            break;
        }
        message = "finally \n";
        send(socketf,message.c_str(),maxPacketSize,0);
    }
    data = nullptr;
    pthread_exit(NULL);
}


server::server(const server& orig) {
}

server::~server() {
}

