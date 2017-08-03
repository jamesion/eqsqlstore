#include "stdafx.h"
#include "Ceqmyslq.h"
#include "eqemu_logsys.h"

//EQEmuLogSys LogSys;
#pragma once
Ceqmyslq::Ceqmyslq()
{
	dessql = mysql_init((MYSQL*)0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "��ʼ��Ŀ�����ݿ����");
		mysql_close(dessql);
		return ;
	}

	scsql = mysql_init((MYSQL*)0);
	if (scsql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "��ʼ��Դ���ݿ����");
		mysql_close(scsql);
		return ;
	}
}


Ceqmyslq::~Ceqmyslq()
{
	if (dessql != NULL)
		mysql_close(dessql);

	if (scsql != NULL)
		mysql_close(scsql);

}

//�������ݿ�����
bool Ceqmyslq::mysql_connect(hostinfo des_host, hostinfo sc_host)
{
	Log(Logs::General, Logs::Normal, "�������ݿ�....");

	deshost = des_host;
	schost = sc_host;

	dessql = mysql_init((MYSQL*)0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "��ʼ��Ŀ�����ݿ����");
		mysql_close(dessql);
		return FALSE;
	}
	dessql = mysql_real_connect(dessql, des_host.hostip, des_host.username,des_host.pwd, des_host.store, atoi(des_host.port), NULL, 0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "����Ŀ�����ݿ������˶�Ŀ�����ݿ���Ϣ��");
		mysql_close(dessql);
		return FALSE;

	}

	scsql = mysql_init((MYSQL*)0);
	if (scsql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "��ʼ��Դ���ݿ����");
		mysql_close(scsql);
		return FALSE;
	}

	scsql = mysql_real_connect(scsql, sc_host.hostip, sc_host.username, sc_host.pwd, sc_host.store, atoi(sc_host.port), NULL, 0);
	if (scsql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "����Դ���ݿ������˶�Դ���ݿ���Ϣ��");
		mysql_close(scsql);
		return FALSE;
	}

	if ((mysql_set_character_set(dessql, "GBK") != 0)&&(mysql_set_character_set(scsql, "GBK") != 0))

	{

		Log(Logs::General, Logs::MysqlErro, "����GBK�ַ�������");

		return FALSE;

	}

	if (mysql_select_db(dessql, des_host.store))
	{
		Log(Logs::General, Logs::MysqlErro, "����Ŀ������");
		mysql_close(dessql);
		return FALSE;
	}
	dessql->reconnect = 1;

	if (mysql_select_db(scsql, sc_host.store))
	{
		Log(Logs::General, Logs::MysqlErro, "����Դ�����");
		mysql_close(scsql);
		return FALSE;
	}
	scsql->reconnect = 1;

	Log(Logs::General, Logs::Normal, "���ݿ�������ɣ�");

	return TRUE;
}


//��ȡ���ݿ���Ϣ��b_dessql=falseΪԴ���ݿ⣬trueΪĿ�����ݿ�
string Ceqmyslq::runSQLCommand(CString sql,BOOL b_dessql)
{
	if ((!dessql->reconnect)&&(scsql->reconnect))
	{
		//û�����ӵ�������    
		Log(Logs::General, Logs::MysqlErro, "û�����ݿ����ӣ�");
		return false;
	}
	if (sql.IsEmpty())
	{
		//SQL���Ϊ��    
		Log(Logs::General, Logs::MysqlErro, "SQL�������Ϊ�գ�");
		return false;
	}
	MYSQL *hMysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	string back;
	
	if (b_dessql)
		hMysql = scsql;
	else
		hMysql = dessql;

	unsigned int i, j = 0;

	i = mysql_real_query(hMysql, sql, (unsigned int)strlen(sql));//ִ�в�ѯ    
	if (i < 0)
	{

		back.append( "MYSQL��ѯָ�����");
		back.append(mysql_error(hMysql));

		Log(Logs::General, Logs::MysqlErro, "%s", back.c_str());

		return back;
	}

	res = mysql_store_result(hMysql);
	if (res == NULL)
	{
		back.append("MYSQL��ѯָ�����");
		back.append(mysql_error(hMysql));

		Log(Logs::General, Logs::MysqlErro, "%s", back.c_str());

		return back;
	}


	while ((row = mysql_fetch_row(res)))
	{
		//���������    
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
//	Log(Logs::General, Logs::Normal, "MYSQL��%s", back.c_str());
//	back.append("");
	return back;
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


//���ݹ������ò�����Դ���ݵ�Ŀ�����ݿ⣬�ɹ����ؿ�ֵ.
string Ceqmyslq::CopyData(string rulegroup)
{
	vector<string> talbe;
	string back;
	talbe=stingtovecort(rulegroup);
	if (talbe[0] == "")
	{
		back.append("��ȡ��������,�����Ϊ��.");
		return back;
	}
	
	back=InitEqsql(talbe[0]);

	return back;
}

//��ʼ��EQsql���ݿ�ʹ��Ӧ�ָ�����
string Ceqmyslq::InitEqsql(string table)
{
	CString sql;
	string back;

	sql.Format("select '%s' from information_schema.tables where table_schema='%s' and table_type='base table'", table.c_str(), deshost.store);
	if ((back=runSQLCommand(sql, true))!="")
	{
		return back;
	}

	return back;
}

vector<string> Ceqmyslq::stingtovecort(string rulegroup)
{
	char* fild = ",",*rrg;
	vector<string> backvector;

	if (rulegroup == "")
	{
		backvector.push_back("");
		return backvector;
	}

	backvector.push_back(strtok((char*)rulegroup.c_str(), fild));
	while (true)
	{
		rrg = strtok(NULL, fild);
		if (rrg == NULL)
			break;

		backvector.push_back(rrg);
	}
	return backvector;
}

//��ȡsql����������״̬
bool Ceqmyslq::getConnectionStatus()
{
	
	return dessql->reconnect;
}

//�ر����ݿ�����
void Ceqmyslq::Close()
{
	if ((dessql->reconnect) && (scsql->reconnect))
	{
		mysql_close(dessql);
		mysql_close(scsql);
	}
	if(!(dessql->reconnect) && !(scsql->reconnect))
		Log(Logs::General, Logs::Normal, "�ѶϿ����ݿ����ӣ�");
	else
		Log(Logs::Error, Logs::MysqlErro, "�޷��Ͽ����ݿ����ӣ�");
}
