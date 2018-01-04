#pragma once
#include <string>
#include "table.h"

using namespace std;

class Database
{
	vector<Table> *zoznamTabuliekPrePrihlaseneho;
public:
	Database();
	bool createTable(string pTableName);
	bool findTable(string pTableName);
        string select(string command,string user);
        bool check(string user,Table* table);
	string toString(string pTableName);
	~Database();
};

