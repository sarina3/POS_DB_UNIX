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
//#include "Server.h"
#include <vector>
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
    this->username = tmp;
    str.append(tmp + ";");
    cout << "password:\n";
    cin >> tmp;
    str.append(tmp);
    return str;
}

string Client::sendMessage(string message){
    char buffer[255];
    bzero(&buffer,255);
    
    message = this->username + ";" + message;
    send(socketf,message.c_str(),255,0);
    recv(socketf,&buffer,255,0);
    message = buffer;
    return message;
} 


void Client::work() {
    bool tmp = true;
    string msg = "";
    //cout << "im here" << endl;
    //bzero(&buffer,255);
    getline(cin,msg);
    while(tmp == true){
        msg = "";
        cout << "co chcete poslat";
        cout << endl;
        getline(cin,msg);
        switch(checkInput(msg)){
            case SELECT:
                msg = this->select();
                msg = this->sendMessage(msg);
                break;
            case INSERT:
                msg = this->insert();
                msg = this->sendMessage(msg);
                break;
            case UPDATE:
                msg = this->update();
                msg = this->sendMessage(msg);
                break;
            case DELETE:
                msg = this->deleteFromTable();
                msg = this->sendMessage(msg);
                break;
            case myClose:
                msg = "close";
                msg = this->sendMessage(msg);
                tmp = false;
                break;
            case getTables:
                msg = this->getTablesAll();
                msg = this->sendMessage(msg);
                break;
            case getMyTables:
                msg = this->getTablesMy();
                msg = this->sendMessage(msg);
                break;
            case DropTable:
                msg = this->dropTable();
                msg = this->sendMessage(msg);
                break;
            case chmod:
                msg = this->chmodfunc();
                msg = this->sendMessage(msg);
                break;
            case chmodrev:
                msg = this->chmodreverse();
                msg = this->sendMessage(msg);
                break;
            case EndProgram:
                msg = this->shutDown();
                msg = this->sendMessage(msg);
                tmp = false;
                break;
            case CreateTable:
                this->createTable();
                break;
            default:
                msg = "zly  prikaz";
                break;
        }
        cout << msg << endl;;
        msg == "";
    }
}

string Client::chmodfunc() {
    string command = "CHMOD;";
    string tmp;
    cout << "zadajte tabulku v ktorej chcete upravovat prava:" <<endl;
    getline(cin,tmp);
    command += tmp + ";";
    cout << "zadajte pouzivatelov ktorim chcete pridat prava (oddelovac , napr.: jozef,kubo):"<< endl;
    getline(cin,tmp);
    command += tmp;
    return command;
}

string Client::shutDown() {
    string command = "shutDown";
    return command;
}

string Client::chmodreverse() {
    string command = "CHMOD-;";
    string tmp;
    cout << "zadajte tabulku do ktorej chcete upravovat prava:" <<endl;
    getline(cin,tmp);
    command += tmp + ";";
    cout << "zadajte pouzivatelov ktorim chcete odobrat prava (oddelovac , napr.: jozef,kubo):"<< endl;
    getline(cin,tmp);
    command += tmp;
    return command;
}

string Client::select() {
    string message = "SELECT;";
    string tmp;
    cout << "zadajte co chcete vybrat z tabulky ako oddelovac pouzite , (napr. id,meno):"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte nazov tabulky z ktorej chcete vyberat:"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte podmienky podla ktorych chcete vyberat (nemusite vyplnit, napr.: id == 1):"<<endl;
    getline(cin,tmp);
    if(tmp != ""){
        message += tmp + ";";
    }else{
        message += ";";
    }
    cout << "zadajte stlpec podla ktoreho chcete zoradit vysledky (napr.: id asc):"<<endl;
    getline(cin,tmp);
    if(tmp != ""){
        message += tmp;
    }
    cout << message;
    return message;
}

string Client::insert() {
    string message = "INSERT;";
    string tmp;
    cout << "zadajte nazov tabulky do ktorej chcete vkladat (napr.: TablukaVzor)"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte nazvy stlpcov do ktorych chcete vkladat (musite zadat vsetky NN stlpce) (napr.: id,meno):"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte hodno ty ktore sa maju vlozit do prislusnych stlpcov(musite zadat v tom istom poradi oddelovac je ,)"<<endl;
    getline(cin,tmp);
    message += tmp;
    cout << message << endl;
    return message;
}

string Client::update() {
    string message = "UPDATE;";
    string tmp;
    cout << "zadajte nazov tabulky ktoru chcete upravit"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte nazvi stlpcov ktore chcete upravit ako oddelovac pozite ,"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte nove hodnoty upravovanych stlpcov zadajte v takom poradi ako nazvy stlpcov oddelovac ,"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte podmienky podla ktorich sa maju stlpce upravit (napr.: meno == Jakub)"<<endl;
    getline(cin,tmp);
    message += tmp;
    cout << message << endl;
    return message;
}

string Client::deleteFromTable() {
    string message = "DELETE;";
    string tmp;
    cout << "zadajte nazov tabulky z ktorej chete mazat"<<endl;
    getline(cin,tmp);
    message += tmp + ";";
    cout << "zadajte podmienky pre vymazanie riadku z tabulky"<<endl;
    getline(cin,tmp);
    message += tmp;
    cout << message << endl;
    return message;
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
           // sendMessage(prikaz); // odpoved ci existuje tabulka
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
    vector<string> *notnull = new vector<string>();
    vector<string> *pk = new vector<string>();
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
            notnull->push_back(ss);
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

    int zhoda = 0;
    vector<string> *rows = new vector<string>();
    if(stlpce2->size() > 0) // iba ak user neukoncil program skorej
    {
        cout << "Vlozte riadky do tabulky: [exit] [end] \n";
       
        int i = 0;
        int tt = 0;
        bool kontrola = false;
        while(!end){
            for(string gg : *stlpce2){
                while(!kontrola){
                    cout << "[ " + gg + " ]: ";
                    cin >> prikaz;
                    if(prikaz == "exit" || prikaz == "end"){ 
                        end = true;
                        break;
                    }
                    if(prikaz == "null"){  //kontrola notnull aj ci PK nie je null
                        for(string ss : *notnull){
                            if(ss == gg){
                                prikaz = "nemoze";
                            }
                        }
                    }
                    if(checkTypesOfColums(typColumn->at(i),prikaz)){
                        
                        for(string kk : *pk){
                            if(kk == gg){
                                for(int gg = tt; gg < rows->size();gg+=stlpce2->size()){
                                    if(rows->at(gg) == prikaz)
                                    {
                                        zhoda++;
                                    };
                                }
                            
                        }
                         tt++;   
                        
                        
                    }
                        tt=0;
                        if(zhoda < 1){
                             cout << prikaz;
                             rows->push_back(prikaz);
                             kontrola = true;
                        }else{
                            cout << "DUplicita PK \n";
                        }
                        }
                    zhoda = 0;
                   
                }
                if(end){
                    break;
                }
                kontrola = false;
                i++;  
            }
            cout << "\n riadok uspesne zadany! \n";
            i = i % stlpce2->size(); //aby bola kontrola typov od zaciatku
            
        }
        
    }
    
       
        
        
    int r = 0;
    for(string ss : *rows){
        if(r==stlpce2->size()-1){
            r = -1;
            createTable+=ss+";";
        }else{
            createTable+=ss+",";
        }
        r++;
    }
    //kontrola TEST
    //cout << "test\n";
    //cout << createTable;
    
    //ODOSLANIE NA SERVER TABULKy
    end = true;


    
    
}

string Client::dropTable() {
    string command = "DROP;";
    string tmp = "";
    cout << "zadajte nazov Tabulky ktoru chcete zmazat:"<<endl;
    getline(cin,tmp);
    command += tmp;
    return command;
}

string Client::getTablesAll() {
    string command = "GET";
    return command;
}
string Client::getTablesMy() {
    string command = "GETMY";
    return command;
}

bool Client::checkTypesOfColums(string typ, string prikaz){
    if(prikaz == "nemoze"){
        cout << "zly vstup \n";
        return false;
    }
    if(prikaz == "null"){
        return true;
    }
    
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

bool Client::checDuplicatesOfPKandNotNullRows(string vstup){
    
    return true;
}

Client::Client(const Client& orig) {
}

Client::~Client() {
    
}

