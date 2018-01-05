#include "Database.h"


Database::Database()
{
}

bool Database::createTable(string pTableName)
{
	bool tmp;
	fstream file; // nevytvori ziadny subor ak by neexistoval
	file.open(pTableName + ".txt");
	if (file.is_open())  //ak neexistuje neotvori sa
	{
		cout << "This table name already exists" << endl;
		file.close();
		tmp = false;
	}
	else {
		ofstream file;
		file.open(pTableName + ".txt"); //ak neexistuje vytvori  	
		cout << "TabulkaVytvorena! \n" << endl;
		tmp = true;

	}
	return tmp;
}

bool Database::findTable(string pTableName)
{
	fstream file; // nevytvori ziadny subor ak by neexistoval
	file.open(pTableName + ".txt");
	if (file.is_open())  //ak existuje otvori sa
	{
		file.close();
		return true;
	}
	else {
		cout << "Tabulka neexistuje \n ";
		return false;
	}
}

bool Database::check(string user,Table* table) {
    bool canManage = false;
    for(int i = 0 ; i < table->prava->size() ; i++){
        if(user == table->prava->at(i)){
            canManage = true;
            break;
        }
    }
    return canManage;
}

string Database::select(string command, string user,string usage) {
    string function = "";
    string selecting = "";
    string table = "";
    string conditions = "";
    string orderby = "";
    size_t position = 0;
    try{
    position = command.find(";");
    function = command.substr(0,position);
    command.erase(0,position+1);
    position = 0;
    position = command.find(";");
    selecting = command.substr(0,position);
    command.erase(0,position+1);
    position = 0;
    position = command.find(";");
    table = command.substr(0,position);
    command.erase(0,position+1);
    
    }catch(exception e){
        return "Error: bad syntax";
    }
    position = 0;
    try{
        position = command.find(";");
        conditions = command.substr(0,position);
        command.erase(0,position+1);
        position = 0;
        position = command.find(";");
        orderby = command.substr(0,position);
        command.erase(0,position+1);
    }catch(exception e){

    }
    string result ="";
    stringstream resulttmp;
    Table* tableObj = new Table(table);
    
    
    if(tableObj->initTable()){
        if(selecting == "*"){
            selecting = "";
            for(int i = 0 ; i < tableObj->columns->size();i++){
                if(i == tableObj->columns->size() - 1){
                    selecting += tableObj->columns->at(i);
                    break;
                }
                selecting += tableObj->columns->at(i) + ",";
            }
        }
        if(this->check(user,tableObj)){
            if(conditions != ""){
                vector<string> *conditionParsed = new vector<string>();
                while(1){
                    position = conditions.find(",");
                    if(position != string::npos){
                        conditionParsed->push_back(conditions.substr(0,position));
                        conditions.erase(0,position +1);
                    }else{
                        conditionParsed->push_back(conditions);
                        break;
                    }
                }
                
                for(int i = 0 ; i < conditionParsed->size();i++){
                    string tmp = conditionParsed->at(i);
                    string operand;
                    string column;
                    try{
                    position = tmp.find(" ");
                    column = tmp.substr(0,position);
                    tmp.erase(0,position + 1);
                    position = tmp.find(" ");
                    operand = tmp.substr(0,position);
                    tmp.erase(0,position + 1);
                    }catch(exception e){
                        return "bad syntax";
                    }
                    int index = -1;
                    for (int j = 0 ; j < tableObj->columns->size();j++){
                        if(column == tableObj->columns->at(j)){
                            index = j;
                            break;
                        }
                    }
                    if(index == -1){
                        return "column name doesn't exist";
                    }
                    if(i == 0){
                        for(int j= 0 ; j < tableObj->rowsVector->size() / tableObj->columns->size() ; j++){
                            if(this->porovnaj(tableObj->rowsVector->at(index + j*tableObj->columns->size()),tmp,tableObj->typ->at(index),operand)){
                                resulttmp << j;
                            }
                        }
                        result = resulttmp.str();
                    }else{
                        resulttmp.clear();
                        for(char& j : result){
                            if(this->porovnaj(tableObj->rowsVector->at(index + ((int)j -48)*tableObj->columns->size()),tmp,tableObj->typ->at(index),operand)){
                                resulttmp << j;
                            }
                        }
                        
                        result = resulttmp.str();
                        
                    }
                }
                delete conditionParsed;
            }else{
                resulttmp.clear();
                for(int i = 0 ; i < tableObj->rowsVector->size() / tableObj->columns->size();i++){
                    resulttmp << i;
                }
                result = resulttmp.str();
            }
            if(orderby != ""){
                position = orderby.find(" ");
                if(position != string::npos){
                    string column = orderby.substr(0,position);
                    orderby.erase(0,position +1);
                    int index = -1;
                    for (int i = 0 ; i < tableObj->columns->size();i++){
                        if(column == tableObj->columns->at(i)){
                            index = i;
                            break;
                        }
                    }
                    if(index != -1){
                        if(orderby == "desc"){
                            for(int i = 0 ; i < result.size() - 1 ; i++){
                                for(int j = 0 ; j  < result.size() - i - 1 ;j++){
                                    if(this->porovnaj(tableObj->rowsVector->at(index + ((int)result.at(j) - 48) * tableObj->columns->size()),tableObj->rowsVector->at(index + ((int)result.at(j+1) -48) * tableObj->columns->size()),tableObj->typ->at(index),"<")){
                                        char tmp = result.at(j);
                                        result.at(j) = result.at(j+1);
                                        result.at(j+1) = tmp;
                                    }   
                                }
                            }
                        }else{
                            for(int i = 0 ; i < result.size() - 1 ; i++){
                                for(int j = 0 ; j  < result.size() - i - 1 ;j++){
                                    if(this->porovnaj(tableObj->rowsVector->at(index + ((int)result.at(j) - 48) * tableObj->columns->size()),tableObj->rowsVector->at(index + ((int)result.at(j+1) -48) * tableObj->columns->size()),tableObj->typ->at(index),">")){
                                        char tmp = result.at(j);
                                        result.at(j) = result.at(j+1);
                                        result.at(j+1) = tmp;
                                    }   
                                }
                            }
                        }
                    }
                }
            }
            vector<int> *selectingParsed = new vector<int>();
            int index = -1;
            string column;
            while(1){
                index = -1;
                position = selecting.find(",");
                if(position == string::npos){
                    column = selecting;
                    for(int i = 0 ; i < tableObj->columns->size();i++){
                        if(column == tableObj->columns->at(i)){
                            selectingParsed->push_back(i);
                            index = i;
                            break;
                        }
                    }
                    if(index == -1){
                        return "column name doesn't exist";
                    }
                    break;
                }
                column = selecting.substr(0,position);
                for(int i = 0 ; i < tableObj->columns->size();i++){
                    if(column == tableObj->columns->at(i)){
                        selectingParsed->push_back(i);
                        index = i;
                        break;
                    }
                }
                if(index == -1){
                    return "column name doesn't exist";
                }
                selecting.erase(0,position + 1);
            }
            if(usage == "function"){
                return result;
            }
            string newResult = "";
            for (char& j : result){
                for(int i = 0 ; i < selectingParsed->size() ;i++){
                    newResult += tableObj->rowsVector->at(selectingParsed->at(i) + ((int)j -48)*tableObj->columns->size());
                }  
                newResult += "\n";
            }
            delete selectingParsed;
            return newResult;
        }else{
            return "you don't have permission to do that";
        }
    }
}

string Database::deleteFromTable(string command, string user) {
    size_t position;
    string function;
    string table;
    string condition;
    try{
        position = command.find(";");
        function = command.substr(0,position);
        command.erase(0,position +1);
        position = command.find(";");
        table = command.substr(0,position);
        command.erase(0,position +1);
        condition = command;
    }catch(exception e){
        return "Bad syntax";
    }
    string ids;
    Table *tableObj = new Table(table);
    if(tableObj->initTable()){
        command = "SELECT;*;"+table+";"+condition+";";
        ids = this->select(command,user,"function");
        for(char &j : ids){
            //miloas
        }
    }
}


bool Database::porovnaj(string clenzpola, string clenfixny, string typ,string operand) {
    if(typ == "double"){
        if(operand == ">="){
            if(stod(clenzpola) >=  stod(clenfixny)){
                return true;
            }
        }
        if(operand == "<="){
            if(stod(clenzpola) <=  stod(clenfixny)){
                return true;
            }
        }
        if(operand == "=="){
            if(stod(clenzpola) ==  stod(clenfixny)){
                return true;
            }
        }
        if(operand == ">"){
            if(stod(clenzpola) >  stod(clenfixny)){
                return true;
            }
        }
        if(operand == "<"){
            if(stod(clenzpola) <  stod(clenfixny)){
                return true;
            }
        }
    }
    if(typ == "int"){
        if(operand == ">="){
            if(stoi(clenzpola) >=  stoi(clenfixny)){
                return true;
            }
        }
        if(operand == "<="){
            if(stoi(clenzpola) <=  stoi(clenfixny)){
                return true;
            }
        }
        if(operand == "=="){
            if(stoi(clenzpola) ==  stoi(clenfixny)){
                return true;
            }
        }
        if(operand == ">"){
            if(stoi(clenzpola) >  stoi(clenfixny)){
                return true;
            }
        }
        if(operand == "<"){
            if(stoi(clenzpola) <  stoi(clenfixny)){
                return true;
            }
        }
    }else{
        if(operand == ">="){
            if(clenzpola >= clenfixny){
                return true;
            }
        }
        if(operand == "<="){
            if(clenzpola <=  clenfixny){
                return true;
            }
        }
        
        if(operand == "=="){
            if(clenzpola == clenfixny){
                return true;
            }
        }
        if(operand == ">"){
            if(clenzpola >  clenfixny){
                return true;
            }
        }
        if(operand == "<"){
            if(clenzpola <  clenfixny){
                return true;
            }
        }
    }
    return false;
}


string Database::toString(string pTableName) //ked tabulka existuje
{
	Table *tbl = new Table(pTableName);
	tbl->initTable();
	return tbl->toStringTable();
	
}

Database::~Database()
{
}
