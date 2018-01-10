#pragma once
#include <string>

enum Prikazy //mozne prikazy
{
	CreateTable,
	AlterTable,
	DropTable,
	EndProgram,
        InitTable,
	ZlyPrikaz,
        SELECT,
        UPDATE,
        DELETE,
        INSERT,
        getTables,
        getMyTables,
        chmod,
        chmodrev,
        myClose,
};


inline Prikazy checkInput(std::string command) //ropoznavanie prikazov
{
	if (command == "CreateTable") { return CreateTable; }
	if (command == "AlterTable" || command == "3") { return AlterTable; }
	if (command == "EndProgram" || command == "exit") {	return CreateTable;	}
        if (command == "SELECT"){return SELECT;}
        if (command == "UPDATE"){return UPDATE;}
        if (command == "DELETE"){return DELETE;}
        if (command == "INSERT"){return INSERT;}
        if (command == "GET"){return getTables;}
        if (command == "InitTable"){return InitTable;}
        if (command == "shutDown"){return EndProgram;}
        if (command == "GETMY"){return getMyTables;}
        if (command == "DROP"){return DropTable;}
        if (command == "CHMOD"){return chmod;}
        if (command == "CHMOD-"){return chmodrev;}
        if (command == "close"){return myClose;}
	return Prikazy::ZlyPrikaz;
}
