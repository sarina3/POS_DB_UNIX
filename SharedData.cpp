/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharedData.cpp
 * Author: root
 * 
 * Created on December 24, 2017, 1:45 PM
 */

#include "SharedData.h"

SharedData::SharedData() {
    toDo = new vector<string>();
    threads = new vector<pthread_t*>();
    activeSockets = new vector<int>();
    mutexSockets,mutexThreads,mutexToDo = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_init(&condSockets,NULL);
    pthread_cond_init(&condThreads,NULL);
    pthread_cond_init(&condToDo,NULL);
}

SharedData::SharedData(const SharedData& orig) {
}

SharedData::~SharedData() {
    delete toDo;
    delete activeSockets;
    pthread_mutex_destroy(&mutexSockets);
    pthread_mutex_destroy(&mutexThreads);
    pthread_mutex_destroy(&mutexToDo);
    pthread_cond_destroy(&condSockets);
    pthread_cond_destroy(&condThreads);
    pthread_cond_destroy(&condToDo);
}

