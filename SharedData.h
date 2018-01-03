/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharedData.h
 * Author: root
 *
 * Created on December 24, 2017, 1:45 PM
 */

#ifndef SHAREDDATA_H
#define SHAREDDATA_H
#include <vector>
#include <pthread.h>
#include <string>
using namespace std;
class SharedData {
public:
    SharedData();
    SharedData(const SharedData& orig);
    virtual ~SharedData();
    vector<int> *activeSockets;
    vector<string> *toDo;
    vector<pthread_t*> *threads;
    pthread_mutex_t mutexSockets;
    pthread_mutex_t mutexThreads;
    pthread_mutex_t mutexToDo;
    pthread_cond_t  condSockets;
    pthread_cond_t  condThreads;
    pthread_cond_t  condToDo;
private:
    
};

#endif /* SHAREDDATA_H */

