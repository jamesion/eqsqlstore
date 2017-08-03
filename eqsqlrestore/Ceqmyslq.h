
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
	bool mysql_connect(hostinfo des_host, hostinfo sc_host);


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
	void destroyConnection();
	string InitEqsql(std::string table);
	vector<string> stingtovecort(std::string rulegroup);

	uint32 LocalID, EmuID, CharacterID;
	hostinfo deshost, schost;

public:
	//读取数据库信息，b_dessql=false为源数据库，true为目标数据库
	std::string runSQLCommand(CString sql,BOOL b_dessql=FALSE);

	unsigned int insert(std::string sql);
	vector< vector<string> > getResult();

	//根据规则设置并拷贝源数据到目标数据库，成功返回true,否则返回false.
	std::string CopyData(string rulegroup);

	//初始化EQsql数据库使适应恢复需求

	void Close();
	bool getConnectionStatus();

};

