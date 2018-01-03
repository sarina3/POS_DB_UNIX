/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.cpp
 * Author: root
 * 
 * Created on December 19, 2017, 5:26 PM
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.h"
#include "Server.h"

Client::Client(){
    port = 0;
    socketf = -1;
    adress = "";
}

int Client::connection(int portNumber) {
    socketf = socket(AF_INET,SOCK_STREAM,0);
    
    if(inet_addr(adress.c_str()) == -1){
        
        hostent *host;
        in_addr **addr_list;
        if((host = gethostbyname("localhost")) == NULL){
            cout << "failed to get hostname\n";
            return -1;
        }
        addr_list = (in_addr **)host->h_addr_list;
        for(int i = 0; addr_list[i] != NULL; i++){
            server.sin_addr = *addr_list[i];
	    break;
        }       
    }else{
        server.sin_addr.s_addr = inet_addr(adress.c_str());
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(portNumber);
    //cout << socketf << &server << sizeof(server) << endl;
    int tmp = connect(socketf,(sockaddr * )&server,sizeof(server));
    if(tmp != 0){
        cout << "failed to connect\n";
        return 0;
    }
    //cout << tmp << endl;
    return 1;
    
        
}

void Client::work() {
    bool tmp = true;
    string msg = "";
    //cout << "im here" << endl;
    char buffer[255];
    //bzero(&buffer,255);
    getline(cin,msg);
    while(tmp == true){
        bzero(&buffer,255);
        cout << "co chcete poslat";
        cout << endl;
        
        getline(cin,msg);
        if(msg == "close"){
            tmp = false;
        }
        if(send(socketf,msg.c_str(),255,0) <0){
            cout << "failed to send\n";
        }
        if(recv(socketf,&buffer,sizeof(buffer),0) <0){
            cout << "recieve failed\n";
        }
        if(buffer == "close"){
            break;
        }
        msg = buffer;
        
        cout << msg + "\n";
    }
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

