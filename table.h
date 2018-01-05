#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Table
{
	
public:
	Table(string pTableName);
	
	bool initTable();
	//alters
	bool addColumns(string pNazov, string pTyp); // bolo by dobre locknut mutex aby sem nikto v tomto case nepristupil
	bool delColumn();
        bool writeTableToFile();
	string toStringTable();
	~Table();
        string name;
	vector<string> *prava;
	vector<string> *typPrava;
	vector<string> *columns;
	vector<string> *typ;
	vector<string> *pk;
	vector<string> *notNull;
	string rows;
        vector<string> *rowsVector;
};

