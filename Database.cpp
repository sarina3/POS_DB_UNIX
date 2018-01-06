#include "Database.h"


Database::Database()
{
}

bool Database::createTable(string pTableName)
{   
    size_t position = pTableName.find(";");
    pTableName.erase(0,position + 1);
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
            delete tableObj;
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
        if(check(user,tableObj)){
        command = "SELECT;*;"+table+";"+condition+";";
        ids = this->select(command,user,"function");
            for(char &j : ids){
                int index = (int)j-48;
                for(index; index < (tableObj->rowsVector->size()/tableObj->columns->size()) - 1;index++){
                    for(int i = 0 ; i < tableObj->columns->size(); i++){
                        tableObj->rowsVector->at(i + index*tableObj->columns->size()) = tableObj->rowsVector->at(i + (index + 1)*tableObj->columns->size());
                    }
                }
                
                for(int i = 0 ; i < tableObj->columns->size(); i++){
                    tableObj->rowsVector->pop_back();
                }
            }
        /*if(tableObj->writeTableToFile()){
           return ids.size() + " rows was successfully deleted";
        }else{
            return "something went wrong";
        }*/
        delete tableObj;
        return "preslo";
        }else{
            return "you don't have permission to do that";
        }
        
    }else{
        return "table with that name doesn't exist";
    }
}
//UPDATE;table;id,meno;4,Vlado;priezvisko == foltan
string Database::update(string command, string user) {
    string function;
    string table;
    string columns;
    string values;
    string conditions;
    size_t position;
    try{
        position = command.find(";");
        function = command.substr(0,position);
        command.erase(0,position+1);
        position = command.find(";");
        table = command.substr(0,position);
        command.erase(0,position+1);
        position = command.find(";");
        columns = command.substr(0,position);
        command.erase(0,position+1); position = command.find(";");
        values = command.substr(0,position);
        command.erase(0,position+1);
        conditions = command;
    }catch(exception e){
        return "Bad syntax";
    }
    Table *tableObj = new Table(table);
    if(tableObj->initTable()){
        if(this->check(user,tableObj)){
            command = "SELECT;*;" + table + ";"+conditions+";";
            string indexes = this->select(command,user,"function");
            vector<int> *colmunsParsed = new vector<int>();
            vector<string> *valuesParsed = new vector<string>();
            while(1){
                position = columns.find(",");
                if(position == string::npos){
                    string col = columns;
                    for(int i = 0 ; i < tableObj->columns->size();i++){
                        if(col == tableObj->columns->at(i)){
                            colmunsParsed->push_back(i);
                            break;
                        }
                    }
                    break;
                }
                string col = columns.substr(0,position);
                columns.erase(0,position + 1);
                for(int i = 0 ; i < tableObj->columns->size();i++){
                    if(col == tableObj->columns->at(i)){
                        colmunsParsed->push_back(i);
                        break;
                    }
                }
            }
            while(1){
                position = values.find(",");
                if(position == string::npos){
                    valuesParsed->push_back(values);
                    break;
                }
                valuesParsed->push_back(values.substr(0,position));
                values.erase(0,position +1);
            }
            cout << valuesParsed->size() << colmunsParsed->size() << endl;
            if(valuesParsed->size() == colmunsParsed->size()){
                for(char &j : indexes){
                    for(int i = 0 ; i < colmunsParsed->size();i++){
                        tableObj->rowsVector->at(colmunsParsed->at(i) + ((int)j -48)*tableObj->columns->size()) = valuesParsed->at(i);
                    }
                }
                delete colmunsParsed;
                delete valuesParsed;
                delete tableObj;
                return indexes.size() + "columns was updated successfully";
            }else{
                return "number of columns and number of values to update don't match";
            }
        }
    }
}
//INSERT;table;columns;values
string Database::insert(string command, string user) {
    string function;
    string table;
    string columns;
    string values;
    size_t position;
    try{
        position = command.find(";");
        function = command.substr(0,position);
        command.erase(0,position+1);
        position = command.find(";");
        table = command.substr(0,position);
        command.erase(0,position+1);
        position = command.find(";");
        columns = command.substr(0,position);
        command.erase(0,position+1);
        values = command;
    }catch(exception e){
        return "Bad syntax";
    }
    Table* tableObj = new Table(table);
    if(tableObj->initTable()){
        if(this->check(user,tableObj)){
            vector<string> *columnsParsed = new vector<string>();
            while(1){
                position = columns.find(",");
                if(position == string::npos){
                    columnsParsed->push_back(columns);
                    break;
                }
                columnsParsed->push_back(columns.substr(0,position));
                columns.erase(0,position +1);
            }
            int counter =0;
            int counterNN =0; 
            for(int i = 0 ; i < tableObj->notNull->size();i++){
                if(tableObj->notNull->at(i) != "x"){
                    counterNN++;
                }
                for(string tmp : *columnsParsed){
                    if(tableObj->notNull->at(i) == tmp){
                        counter ++;
                    }
                }
            }
            
            if(counter != counterNN){
                return "not all Not Null columns filled";
            }
            vector<string> *valuesParsed = new vector<string>();
            while(1){
                position = values.find(",");
                if(position == string::npos){
                    valuesParsed->push_back(values);
                    break;
                }
                valuesParsed->push_back(values.substr(0,position));
                values.erase(0,position +1);
            }
            vector<int> *columnsID = new vector<int>();
            for(string iterator : *columnsParsed){
                for(int i = 0 ; i < tableObj->columns->size();i++){
                    if(iterator == tableObj->columns->at(i)){
                        columnsID->push_back(i);
                    }
                }
            }
            for(int i = 0 ; i < columnsID->size();i++){
                for(int j = 0 ; j < columnsID->size()-i-1;j++){
                    if(columnsID->at(j) > columnsID->at(j+1)){
                        int tmp = columnsID->at(j);
                        columnsID->at(j) = columnsID->at(j+1);
                        columnsID->at(j+1) = tmp;
                        string tmp1 = valuesParsed->at(j);
                        valuesParsed->at(j) = valuesParsed->at(j+1);
                        valuesParsed->at(j+1) = tmp1;
                    }
                }
            }
            bool tmp;
            columnsParsed->clear();
            for(int i = 0 ; i < tableObj->columns->size();i++){
                tmp = false;
                for(int j = 0 ; j < columnsID->size();j++){
                   if(i == columnsID->at(j)){
                        columnsParsed->push_back(valuesParsed->at(j));
                        tmp = true;
                        break;
                    }
                }
                if(!tmp){
                    columnsParsed->push_back("");
                }
            }
            cout << endl;
            for(int i = 0; i < columnsParsed->size() ; i++){
                tableObj->rowsVector->push_back(columnsParsed->at(i));
                //cout << i << columnsParsed->at(i);
            }
            stringstream pom;
            for(int i = 0 ; i < tableObj->rowsVector->size();i++){
                if(tableObj->rowsVector->at(i) == ""){
                    pom << "|";
                }else{
                    pom << tableObj->rowsVector->at(i);
                }
            }
            cout << pom.str();
            delete columnsParsed;
            delete columnsID;
            delete valuesParsed;
            delete tableObj;
            return "row was inserted successfully";
        }else{
            return "you dont have permission to do that";
         }
    }else{
        return "table you want to insert into does not exist";
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
