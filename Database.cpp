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

string Database::select(string command, string user) {
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
    Table* tableObj = new Table(table);
    if(tableObj->initTable()){
        if(this->check(user,tableObj)){
            
        }else{
            return "you don't have permission to do that";
        }
    }
}



string Database::toString(string pTableName)
{
	//pridanie moznosti citania zo zasobnika
	//pripad ked tabulka neexistuje 
	Table *tbl = new Table(pTableName);
	tbl->initTable();
	return tbl->toStringTable();
	
}

Database::~Database()
{
}
