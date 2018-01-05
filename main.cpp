/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: root
 *
 * Created on December 19, 2017, 4:37 PM
 */

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include "Server.h"
#include "Client.h"
#include "table.h"
#include "Database.h"
#include "table.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    int tmp = 0;
    cout << "1 - server \n"
            "2 - client \n";
    cin >> tmp;
    //cout << tmp + "\n";
    
    
    if(tmp == 1){
        server *serv = new server(1026);
        serv->something();
    }if(tmp == 2){
        Client *cli = new Client();
        //cli->createTable(); //MIlos testovanie 
        //cout << "main";
        //cout << cli->connection(1027) << endl;
        //cout << "main alive \n ";
        if(cli->connection(1026) == 1){
            cli->work();
        }
    }
    /*
    Database *data = new Database();
    //cout << data->toString("TabulkaVzor");
    //vytvaranie tabulky
    //data->createTable("test");
    
    cout << data->select("SELECT;*;TabulkaVzor;;id desc","admin","");
    */
    return 0;
}

