#include "stdafx.h"
#include "Ceqmyslq.h"
#include "eqemu_logsys.h"

//EQEmuLogSys LogSys;
#pragma once
Ceqmyslq::Ceqmyslq()
{
}


Ceqmyslq::~Ceqmyslq()
{
	if (dessql != NULL)
		mysql_close(dessql);

	if (scsql != NULL)
		mysql_close(scsql);

}

//�������ݿ�����
bool Ceqmyslq::mysql_connect(hostinfo deshost, hostinfo schost)
{
	Log(Logs::General, Logs::Normal, "�������ݿ�....");


	dessql = mysql_init((MYSQL*)0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "��ʼ��Ŀ�����ݿ����");
		mysql_close(dessql);
		return FALSE;
	}
	dessql = mysql_real_connect(dessql, deshost.hostip, deshost.username,deshost.pwd, deshost.store, atoi(deshost.port), NULL, 0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "����Ŀ�����ݿ����%d,ip:%s %d", mysql_error(dessql), deshost.hostip,deshost.port);
		return FALSE;

	}

	scsql = mysql_init((MYSQL*)0);
	scsql = mysql_real_connect(scsql, schost.hostip, schost.username, schost.pwd, schost.store, atoi(schost.port), NULL, 0);
	if (scsql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "����Դ���ݿ����%d", mysql_error(scsql));
		mysql_close(scsql);
		return FALSE;
	}

	if ((mysql_set_character_set(dessql, "GBK") != 0)&&(mysql_set_character_set(scsql, "GBK") != 0))

	{

		Log(Logs::General, Logs::MysqlErro, "����GBK�ַ�������");

		return FALSE;

	}

	if (mysql_select_db(dessql, deshost.store))
	{
		Log(Logs::General, Logs::MysqlErro, "����Ŀ������");
		return FALSE;
	}
	dessql->reconnect = 1;

	if (mysql_select_db(scsql, schost.store))
	{
		Log(Logs::General, Logs::MysqlErro, "����Դ�����");
		return FALSE;
	}
	scsql->reconnect = 1;

	Log(Logs::General, Logs::Normal, "���ݿ�������ɣ�");




	return TRUE;
}

bool Ceqmyslq::runSQLCommand(CString sql)
{
	if ((!dessql->reconnect)&&(scsql->reconnect))
	{//    û�����ӵ�������    
		Log(Logs::General, Logs::MysqlErro, "û�����ݿ����ӣ�");
		return false;
	}
	if (sql.IsEmpty())
	{//    SQL���Ϊ��    
		Log(Logs::General, Logs::MysqlErro, "SQL����Ϊ�գ�");
		return false;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;
	
	unsigned int i, j = 0;

	i = mysql_real_query(dessql, sql, (unsigned int)strlen(sql));//    ִ�в�ѯ    
	if (i < 0)
	{
		Log(Logs::General, Logs::MysqlErro, "MYSQL��ѯָ�����%s",mysql_error(dessql));
		return false;
	}

	res = mysql_store_result(dessql);
	if (res == NULL)
	{
		Log(Logs::General, Logs::MysqlErro, "MYSQL��ѯָ�����%s", mysql_error(dessql));
		return false;
	}


	while ((row = mysql_fetch_row(res)))
	{//    ���������    
		objectValue.clear();
		//j = 1;
		for (j = 0; j < mysql_num_fields(res); j++)
		{
			if (row[j] == NULL)
				objectValue.push_back("");
			else
			objectValue.push_back(row[j]);
		}
		//Log(Logs::General, Logs::Normal, "MYSQL��%s", objectValue.back());

		resultList.push_back(objectValue);
	}
	mysql_free_result(res);   //free result after you get the result    

//	for(int i=0;i<resultList.size();++i)
//		Log(Logs::General, Logs::Normal, "MYSQL��%s", resultList[i][0].c_str());

	return true;
}

unsigned int Ceqmyslq::insert(std::string sql)
{
	if ((!dessql->reconnect) && (scsql->reconnect)) {
		Log(Logs::General, Logs::MysqlErro, "û�����ݿ����ӣ�");
		return -1;
	}
	if (sql.empty()) {
		Log(Logs::General, Logs::MysqlErro, "SQL����Ϊ�գ�");
		return -1;
	}
	int rows = -1;
	int res = mysql_query(dessql, sql.c_str());
	if (res >= 0) {
		// ������Ӱ�������
		rows = (int)mysql_affected_rows(dessql);
		Log(Logs::General, Logs::MysqlErro, "������Ӱ��������%d",row);
		return rows;
	}
	else {
		Log(Logs::General, Logs::MysqlErro, "SQL�����������");
		return -1;
	}
}

vector< vector<string> > Ceqmyslq::getResult()
{

	vector<vector<string> >result;
	result.swap(resultList);
	return result;
}

void Ceqmyslq::destroyConnection()
{
	mysql_close(dessql);
	dessql->reconnect = false;
}

bool Ceqmyslq::getConnectionStatus()
{
	dessql->reconnect;
	return true;
}

//�ر����ݿ�����
void Ceqmyslq::Close()
{
	mysql_close(dessql);
	mysql_close(scsql);
	Log(Logs::General, Logs::Normal, "�ѶϿ����ݿ����ӣ�");
}
