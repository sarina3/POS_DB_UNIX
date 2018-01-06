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
#include "Commands.h"

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
    char buff[255];
    string str = this->login();
    send(socketf,str.c_str(),255,0);
    bzero(buff,255);
    recv(socketf,&buff,255,0);
    str = buff;
    if(str == "wrong"){
        cout << "failed to connect due to bad username or password \n";
        return 0;
    }
    //cout << tmp << endl;
    return 1;
    
        
}

string Client::login() {
    string str = "";
    string tmp;
    cout << "Log in Foltan&Sarina&Ivanic's database:\nUsername:\n";
    cin >> tmp;
    str.append(tmp + ";");
    cout << "password:\n";
    cin >> tmp;
    str.append(tmp);
    return str;
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
        msg = "admin;" + msg;
        if(msg == "close"){
            tmp = false;
        }
        send(socketf,msg.c_str(),255,0);
        recv(socketf,&buffer,255,0);
        if(buffer == "close"){
            break;
        }
        msg = buffer;
        
        cout << msg + "\n";
    }
}

void Client::createTable(){
    string createTable = "";
    string prikaz = "";
    string odpovedServer = "";
    while(1) //opakuje sa pokym nie je zadane meno tabulky ktore neexistuje alebo nie je napisane EXIT
    {
        cout << "[VyvaranieTabulky]: Zadajte nazov tabulky alebo ukoncite [EXIT] \n";
        cin >> prikaz;
        if(prikaz == "EXIT" || prikaz == "exit"){return;} //ked stlaci exit koncime hned
        if(prikaz == "end"){break;} //len na testovanie
        if(prikaz != ""){
            //POZIADANIE O ODPOVED SERVERA
            if(odpovedServer == "OK"){
                cout << "Tabulka uspesne vytvorena! \n";
                break;
            }
        }
    }
    //tu uz je tabulka vytvorena 
    //vyskladanie stringu
    createTable = "prava,";
    int pocetUserovPrava = 0;
    bool end = false;
    vector<string> *stlpce2 = new vector<string>();
    vector<string> *typColumn = new vector<string>();
    while(!end){
        cout << "zadajte prava k tabulke: [exit] [end]\n";
        cout << "Vsetci useri: [  ]\n"; //DOTAZ NA SERVER O VYPISANIE USEROV
        vector<string> *pom = new vector<string>();
        for(int i = 1;;i++)
        {
            cout << "Pravo " + to_string(i) + ": ";
            cin >> prikaz;
            if(prikaz == "exit"){ 
                end = true;
                break;
            }
            if(prikaz == "end"){
                break;
            }
            if(prikaz.find(",") == string::npos ){  //mozno pridat kontrolu medzerz
                createTable = createTable + prikaz + ","; 
                //ci existuje dany user
                pom->push_back(prikaz);
            }else{
                cout << "neplatny vstup\n";
                i--;
            }  
        }
        //zadavanie typu prava
        createTable = createTable + "typPrava,";
        cout << "Prava k tabulke(x+r+w) : \n";
        bool knt = false;
        for(string ss : *pom){
            cout << "user: [" + ss + "]: ";
            while(!knt){
                cin >> prikaz;
                if(prikaz == "exit"){ 
                    end = true;
                    break;
                }
                if(prikaz == "end"){
                    break;
                }
                if(prikaz == "x+r+w"  || prikaz == "x+r" || prikaz == "x+w" || prikaz == "x" 
                        || prikaz == "x+w+r" || prikaz == "w+r" || prikaz == "w+x" || prikaz == "x" || 
                        prikaz == "r+w+x" || prikaz == "r" || prikaz == "r+w" || prikaz == "x+w" )
                {
                    knt = true;
                    createTable = createTable + prikaz + ",";
                }else{
                    cout << "zadajte prava este raz \n";
                }
            }
            
        }
        //zadavanie columnov
        createTable = createTable + "columns,";
        vector<string> *stlpce = new vector<string>();
        cout << "Zadajte stlpce: \n";
        for(int i = 1;;i++)
        {
            cout << "Stlpec" + to_string(i) + ": ";
            cin >> prikaz;
            if(prikaz == "exit"){ 
                end = true;
                break;
            }
            if(prikaz == "end"){
                break;
            }
            stlpce->push_back(prikaz);
            createTable+=prikaz + ",";
        }
        //typ columnov
        createTable+="typ,";
        cout << "Zadajte typ stlpcov [int] [boolean] [string] [date] [double] \n";
        for(string ss : *stlpce){
            while(1){
                cout << "Stlpec [" + ss + "] : ";
                cin >> prikaz;
                if(prikaz == "exit"){ 
                    end = true;
                    break;
                }
                if(prikaz == "end"){
                    break;
                }
                if(prikaz == "string" || prikaz == "int" || prikaz == "boolean" || prikaz == "date" || prikaz == "double") //jednoducha kontrola vstupu
                {
                    createTable += prikaz + ","; 
                    typColumn->push_back(prikaz);
                    break;
                }else{
                    cout << "zly typ \n";
                }
            }
        }
        //PK
        createTable+= "PK,";        
        cout << "Vyberte PK [x] NO [y] YES \n";
        string tmp = "";
        vector<string> *pk = new vector<string>();
        if(stlpce->size() > 0 && stlpce != nullptr){ 
            tmp = stlpce->at(0);
        }
        for(string ss : *stlpce){
            cout << "Stlpec [" + ss + "] :";
            cin >> prikaz;
            if(prikaz == "exit"){ 
                end = true;
                break;
            }
            if(prikaz == "end"){
                break;
            }
            if(prikaz == "y")
            {
                pk->push_back(ss);
                createTable+=ss + ",";
            } 
        }
        //kontrola ci je vytvoreny kluc
        if(pk->size() < 1){
             //vytvorenie umeleho kluca cim je prvy stlpec tabulky
            pk->push_back(tmp);
            createTable+=tmp + ",";
        }
        //kontrola duplicit aby sa nezobrazovali stlpce pk
        vector<string> *notnull = new vector<string>();
        
        
        int jk = 0;
        for(string gg : *stlpce){
            stlpce2->push_back(gg);
            for(string ss : *pk){
                if(ss == gg){
                    stlpce->at(jk) = "";
                }
            }
            jk++;
        }
        //NN
        createTable+="NN,";
        cout << "Zadajte Not Null stlpce [x] null [y] NotNull (Stlpce PK su automaticky NN) : \n";
        for(string ss : *stlpce){
            if(ss != "")
            {
                cout << "Stlpec [" + ss + "] : ";
                cin >> prikaz;
                if(prikaz == "exit"){ 
                    end = true;
                    break;
                }
                if(prikaz == "end"){
                    break;
                }
                if(prikaz == "y")
                {
                    notnull->push_back(ss);
                }else{
                    notnull->push_back("");
                }
            }
        }
        
        for(string ss : *pk){
            createTable+= ss + ",";
        }
        for(string ss : *notnull){
            if(ss != ""){
                createTable+= ss + ",";
            }
            
        }
        createTable+="rows,";
        //zadavanie rows
        //tu uz je hotova tabulka
        end = true;
    }
    end = false;
    if(stlpce2->size() > 0) // iba ak user neukoncil program skorej
    {
        cout << "Vlozte riadky do tabulky: [exit] [end] \n";
        vector<string> *rows = new vector<string>();
        int i = 0;
        bool kontrola = false;
        while(!end){
            for(string gg : *stlpce2){
                while(!kontrola){
                    cout << "[ " + gg + " ]: ";
                    cin >> prikaz;
                    if(prikaz == "exit"){ 
                        end = true;
                        break;
                    }
                    if(prikaz == "end"){
                        break;
                    }
                    if(checkTypesOfColums(typColumn->at(i),prikaz)){
                        cout << prikaz;
                        rows->push_back(prikaz);
                        kontrola = true;
                        
                    }
                }
                kontrola = false;
                i++;  
            }
            cout << "\n riadok uspesne zadany! \n";
            i = i % stlpce2->size(); //aby bola kontrola typov od zaciatku
            
        }
        
    }
    
       
        
        
        
        
        //kontrola TEST
        cout << "test\n";
        cout << createTable;
        end = true;
        

    
    
}

bool Client::checkTypesOfColums(string typ, string prikaz){
    try
    {
        //STRING netreba kontrolovat ten je ok
        if(typ == "int"){int ss = stoi(prikaz);} 
        if(typ == "double"){double ss = stod(prikaz);} 
        if(typ == "boolean" && (prikaz == "true" || prikaz == "false")){return true;}
        if(typ == "date"){
            int den = stoi(prikaz.substr(0,2));
            den = stoi(prikaz.substr(3,2));
            int rok = stoi(prikaz.substr(6,4));
            if(rok > 2020 || rok < 0){ //rok pana >D
                return false;
            }

        } 
    }
    catch(exception e)
    {
        cout << "zly vstup\n";
        return false;
    }
        
    return true;
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

