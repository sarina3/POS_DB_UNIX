
#include "table.h"
#include <errno.h>


Table::Table(string pTableName) //nevolat konstruktor, volat cez Database
{
	name = pTableName;
	prava = new vector<string>();
	typPrava = new vector<string>();
	columns = new vector<string>();
	typ = new vector<string>();
	pk = new vector<string>();
	notNull = new vector<string>();
        rowsVector = new vector<string>();
}

bool Table::initTable()
{
	ifstream file;
	string ch = "";
	file.open(name + ".txt");
	getline(file, ch, ',');
	if (ch == "") { //nacita prve slovo, ak tam neni je prazda
		return false;
	}

	string tmp = "";
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "typPrava") {
			break;
		}
		prava->push_back(tmp); //prava
	}
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "columns") { break; }
		typPrava->push_back(tmp); //stplce
	}
	//typy
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "typ") { break; }
		columns->push_back(tmp);
	}
	//PK
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "PK") { break; }
		typ->push_back(tmp); //stplce
	}
	//NN
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "NN") { break; }
		pk->push_back(tmp); //stplce
	}
	//NN -- 
	while (1)
	{
		getline(file, tmp, ',');
		if (tmp == "rows") { break; }
		notNull->push_back(tmp); //stplce
	}
	while (!file.eof())
	{
		getline(file, tmp, ';');
		rows += tmp;
                while(1){
                    size_t pos = tmp.find(",");
                    if(pos == string::npos){
                        rowsVector->push_back(tmp);
                        break;
                    }
                    rowsVector->push_back(tmp.substr(0,pos));
                    tmp.erase(0,pos +1);
                }
	}
	return true;
}


bool Table::addColumns(string pNazov, string pTyp)
{
	return false;
}

bool Table::delColumn()
{
	return false;
}

string Table::toStringTable()
{
	string tmp = name + "\n prava: ";
	for(string var : *prava)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n typPrava: ";
	for (string var : *typPrava)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n columns: ";
	for (string var : *columns)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n typ: ";
	for (string var : *typ)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n PK: ";
	for (string var : *pk)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n NN: ";
	for (string var : *notNull)
	{
		tmp = tmp + var + " | ";
	}
	tmp = tmp + "\n rows: ";
	//tmp = tmp + rows;
        int pocet = 0;
        for(string var : *rowsVector){
            if(pocet == columns->size()){
                pocet = 0;
                tmp += ";\n";
            }
            tmp += var+",";
            pocet++;
        }
	return tmp;
}

bool Table::writeTableToFile() //metoda sa da volat len po poriadnom vytvoreni tabulky
{ 
    ofstream file;
    file.open(this->name + ".txt"); //vytvorenie suboru
    string tmp = name + "prava,";
	for(string var : *prava)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "typPrava,";
	for (string var : *typPrava)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "columns,";
	for (string var : *columns)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "typ,";
	for (string var : *typ)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "PK,";
	for (string var : *pk)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "NN,";
	for (string var : *notNull)
	{
		tmp = tmp + var + ",";
	}
	tmp = tmp + "rows,";
	tmp = tmp + rows;
        int pocet = 0;
        for(string var : *rowsVector){
            if(pocet == columns->size()){
                pocet = 0;
                tmp += ";";
            }
            tmp += var;
            pocet++;
        }
	file << tmp;
        file.close();
}

Table::~Table()
{
}
