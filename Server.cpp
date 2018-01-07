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
#include "Logins.h"
#include "Server.h"
#include "Commands.h"
#include "Database.h"
#include <iostream>

server::server(int portNumber) {
    
    this->database = new Database();
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
    string toDo;
    size_t position;
    string user;
    int socketNum;
    bool noReturn;
    while(1){
        toDo = "";
        noReturn = false;
        pthread_mutex_lock(&this->data->mutexToDo);
        
        if(this->data->toDo->size() == 0){
            //cout << "cakam" << endl;
            pthread_cond_wait(&this->data->condToDo,&this->data->mutexToDo);
        }
        toDo = this->data->toDo->at(0);
        //cout << this->data->toDo->size();
        for(int i = 0 ; i < this->data->toDo->size() ; i++){
           // cout << this->data->toDo->at(i);
            if(i != this->data->toDo->size() - 1 ){
                this->data->toDo->at(i) = data->toDo->at( i+1);
            }else{
                this->data->toDo->pop_back();
            }
        }
        pthread_mutex_unlock(&this->data->mutexToDo);
        //cout << toDo << endl;
        position = toDo.find(";");
        socketNum = stoi(toDo.substr(0,position));
        //cout << socketNum << endl;
        toDo = toDo.erase(0,position + 1);
        position = 0;
        position = toDo.find(";");
        user = toDo.substr(0,position);
        toDo.erase(0,position+1);
        position = 0;
        position = toDo.find(";");
        //cout << toDo << endl;
        switch(checkInput(toDo.substr(0,position))){
            case SELECT:
                toDo = this->database->select(toDo,user,"");
                break;
            case CreateTable:
                toDo = this->database->createTable(toDo); //navratova hodnota pre klienta,
                break;
            case INSERT:
                toDo = this->database->insert(toDo,user);
                break;
            case myClose:
                this->Myclose(socketNum);
                noReturn = true;
                break;
            case UPDATE:
                toDo = this->database->update(toDo,user);
                break;
            case DELETE:
                toDo = this->database->deleteFromTable(toDo,user);
                break;
            case DropTable:
                toDo = this->database->dropTable(toDo,user);
                break;
            case getTables:
                toDo = this->database->getAllTables(toDo);
                break;
            default:
                toDo = "command you have entered does not exist";
                break;
        }
        // sem pride bud switch alebo daco ine co bude volat funkcie  podla prikazu v Stringu
        if(!noReturn){
        stringstream str;
        str << socketNum << ";";
        toDo = str.str() + toDo;
        pthread_mutex_lock(&this->data->mutexResults);
        this->data->results->push_back(toDo);
        pthread_cond_broadcast(&this->data->condResults);
        pthread_mutex_unlock(&this->data->mutexResults);
        }
    }
}
void server::Myclose(int socketnum) {
    pthread_mutex_lock(&this->data->mutexSockets);
    int index;
    for(int i = 0 ; i < data->activeSockets->size() ;i++){
        if(socketnum == data->activeSockets->at(i)){
            index = i;
        }
    }
    for(int i = index ; index < data->activeSockets->size() -1 ; i++){
        data->activeSockets->at(i) = data->activeSockets->at(i+1);
    }
    data->activeSockets->pop_back();    
    //pthread_t *tmp;
    pthread_mutex_unlock(&data->mutexSockets);
    pthread_mutex_lock(&data->mutexThreads);
    //tmp = data->threads->at(index);
    //pthread_cancel(*tmp);
    for(int i = index ; i < data->threads->size() -1 ;i++){
        data->threads->at(i) = data->threads->at(i+1);
    }
    data->threads->pop_back();
    pthread_mutex_unlock(&data->mutexThreads);
    //delete tmp;
    close(socketnum);
    cout << "conection with number" << socketnum << "was closed successfully" << endl;
}

void* server::listening(void* pdata){
    SharedData *data = (SharedData*)(pdata);
    int socketf,newsocket;
    Logins *log = new Logins();
    if(data ->activeSockets->size() > 0){
        pthread_mutex_lock(&data->mutexSockets);
        socketf = data->activeSockets->at(data->activeSockets->size() - 1);
        pthread_mutex_unlock(&data->mutexSockets);
    }else{
        return 0;
    }
    sockaddr_in  server_addr,client_addr;
    char buffer[255];
    string str;
    while(1){
    listen(socketf,5);
    socklen_t clientAddrSize = sizeof(client_addr);
    newsocket = accept(socketf,(sockaddr*)&client_addr,&clientAddrSize);
    recv(newsocket,&buffer,255,0);
    str = buffer;
    size_t position = str.find(";");
    if(position != string::npos){
        string username = str.substr(0,position);
        str = str.erase(0,position + 1);
        string passwd = str;
        cout << username << endl << passwd << endl;
        if(log->checkUserAndPass(username,passwd)){
            str = "succeed";
            send(newsocket,str.c_str(),255,0);
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
        }else {
            str = "wrong";
            send(newsocket,str.c_str(),255,0);
            close(newsocket);
        }
    }else{
        str = "wrong";
        send(newsocket,str.c_str(),255,0);
        close(newsocket);
    }
    
    
    }
    data = NULL;
    pthread_exit(NULL);
}
void* server::work(void* pdata){
    //cout << "server alive\n";
    SharedData * data = (SharedData*)pdata;
    string message;
    int maxPacketSize = 255;
    int socketf;
    pthread_mutex_lock(&data->mutexSockets);
    if(data ->activeSockets->size() > 0){
        
        socketf = data->activeSockets->at(data->activeSockets->size() - 1);
        pthread_mutex_unlock(&data->mutexSockets);
    }else{
        pthread_mutex_unlock(&data->mutexSockets);
        return 0;
    }

    char msg[255];
    int n = 0;  
    size_t position;
    bool close = false;
    while(1){
        message = "" ;
        n = recv(socketf,msg,maxPacketSize,0);
        message = msg;
        position = message.find(";");
        message.erase(0,position+1);
        cout << message << endl;
        if(message == "close"){
            cout << "som tu" << endl;
            message = "closing connection";
            send(socketf,message.c_str(),maxPacketSize,0);
            close = true;
        }
        message = msg;
        stringstream str;
        str << socketf << ";";
        message = str.str() + message;
        pthread_mutex_lock(&data->mutexToDo);
        data->toDo->push_back(message);
        pthread_cond_signal(&data->condToDo);
        pthread_mutex_unlock(&data->mutexToDo);
        if(close){
            break;
        }
        pthread_mutex_lock(&data->mutexResults);
        if(data->results->size() >= 0){
            
            pthread_cond_wait(&data->condResults,&data->mutexResults);
            
        }
        bool pulled = false;
        for(int i  = 0; i < data->results->size() ; i++){
            string iter = data->results->at(i);
            size_t position = iter.find(";");
            if(stoi(iter.substr(0,position)) == socketf){
                iter.erase(0,position + 1);
                message = iter;
                pulled = true;
            }
            if(pulled){
                if(i != data->results->size() - 1){
                    data->results->at(i) = data->results->at(i+1);
                }else{
                    data->results->pop_back();
                }
            }
        }
        pthread_mutex_unlock(&data->mutexResults);
        send(socketf,message.c_str(),maxPacketSize,0);
        
    }
    cout << "koncim" << endl;
    data = NULL;
    pthread_cancel(pthread_self());
    
}


server::server(const server& orig) {
}

server::~server() {
}

