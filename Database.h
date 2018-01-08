#pragma once
#include <string>
#include "table.h"
#include <sstream>


using namespace std;

class Database
{
	
public:
	Database();
	bool createTable(string pTableName);
	bool findTable(string pTableName);
        string select(string command,string user,string usage);
        string deleteFromTable(string commmand, string user);
        string dropTable(string command,string user);
        string getAllTables(string command);
        string getMyTables(string command,string user);
        string update(string command,string user);
        string insert(string command,string user);
        string chmod(string command,string user);
        string chmodrev(string command, string user);
        bool check(string user,Table* table,bool owner);
        bool porovnaj(string clenzpola, string clenfixny, string typ,string operand);
	string toString(string pTableName);
	~Database();
};

