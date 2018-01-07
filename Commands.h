#pragma once
#include <string>

enum Prikazy //mozne prikazy
{
	CreateTable,
	AlterTable,
	DropTable,
	EndProgram,
	ZlyPrikaz,
        SELECT,
        UPDATE,
        DELETE,
        INSERT,
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
        if (command == "close"){return myClose;}
	return Prikazy::ZlyPrikaz;
}
