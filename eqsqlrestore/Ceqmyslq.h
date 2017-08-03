
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
	//��ȡ���ݿ���Ϣ��b_dessql=falseΪԴ���ݿ⣬trueΪĿ�����ݿ�
	std::string runSQLCommand(CString sql,BOOL b_dessql=FALSE);

	unsigned int insert(std::string sql);
	vector< vector<string> > getResult();

	//���ݹ������ò�����Դ���ݵ�Ŀ�����ݿ⣬�ɹ�����true,���򷵻�false.
	std::string CopyData(string rulegroup);

	//��ʼ��EQsql���ݿ�ʹ��Ӧ�ָ�����

	void Close();
	bool getConnectionStatus();

};

