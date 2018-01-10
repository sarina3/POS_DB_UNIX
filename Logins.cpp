

#include "Logins.h"
using namespace std;

Logins::Logins()
{
	logins = new vector<string>();
	passwords = new vector<string>();
	try
	{
		loadLoginsTXT();
	}
	catch (const std::exception& ex)
	{
		cerr << ex.what() << endl;
	}
	
}

bool Logins::findUser(string pUsername)
{
	for (string pom : *logins)
	{
		if (pom == pUsername) {
			return true;
		}
	}
	return false;
}

bool Logins::loadLoginsTXT()
{
	ifstream logInsFile;
	logInsFile.open("logins.TXT");
        size_t position;
	while(!logInsFile.eof())
	{
		string tmpUsername, tmpPassword = "";
		getline(logInsFile, tmpUsername, ';');
		getline(logInsFile, tmpPassword, ';');
                position = tmpUsername.find("\n");
                if(position != string::npos){
                    tmpUsername.erase(position,1);
                }
                if(tmpUsername == ""){
                    break;
                }
		logins->push_back(tmpUsername);
		passwords->push_back(tmpPassword);
	}
        for(int i = 0 ; i < logins->size(); i++){
            cout << i << logins->at(i) << passwords->at(i)<<endl;
        }
	return true;
}

bool Logins::checkUserAndPass(string pUsername,string pPassword)
{
	int i = 0;
        bool exist = false;
        bool existUser = false;

	for (string pom : *logins)
	{
            if(pom == pUsername){
                existUser = true;
            }
            if (pom == pUsername && pPassword == (*passwords).at(i)) { //passwords

                    exist = true;
                    break;
            }
            i++;
	}
	if(!exist && !existUser){
            ofstream file;
            file.open("logins.TXT");
            for(int i = 0 ; i < this->logins->size();i++){
                file << this->logins->at(i) << ";" << this->passwords->at(i) << ";\n";
            }
            file << pUsername<<";"<<pPassword<<";\n";
            file.close();
            exist = true;
        }
        return exist;

}

Logins::~Logins()
{
    delete  logins;
    delete  passwords;
}
