#pragma once
#include <string>
#include "table.h"
#include <sstream>

using namespace std;

class Database
{
	vector<Table> *zoznamTabuliekPrePrihlaseneho;
public:
	Database();
	bool createTable(string pTableName);
	bool findTable(string pTableName);
        string select(string command,string user,string usage);
        string deleteFromTable(string commmand, string user);
        string update(string command,string user);
        bool check(string user,Table* table);
        bool porovnaj(string clenzpola, string clenfixny, string typ,string operand);
	string toString(string pTableName);
	~Database();
};

