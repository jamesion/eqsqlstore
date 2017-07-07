
#include <windows.h>
#include "stdio.h"
#include "winsock.h"
#include "mysql.h"
#include <string>
#include "types.h"
#include <iostream>    
#include <vector>
//#include <string.h>    

using namespace std;
#pragma once
class Ceqmyslq
{
public:
	Ceqmyslq();
	~Ceqmyslq();
	bool mysql_connect(hostinfo deshost,hostinfo schost);


private:

	MYSQL* dessql, *scsql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	CString temp;
	char *query = NULL;
	int rt;
	unsigned int t;
	int count = 0;
	vector< vector<string> > resultList;
	vector<string> objectValue;


public:
	bool runSQLCommand(string sql);
	unsigned int insert(std::string sql);
	vector< vector<string> > getResult();
	void destroyConnection();
	bool getConnectionStatus();
	void Close();
};

