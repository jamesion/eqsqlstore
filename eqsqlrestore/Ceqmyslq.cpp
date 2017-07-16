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

//建立数据库连接
bool Ceqmyslq::mysql_connect(hostinfo deshost, hostinfo schost)
{
	Log(Logs::General, Logs::Normal, "连接数据库....");


	dessql = mysql_init((MYSQL*)0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "初始化目标数据库错误！");
		mysql_close(dessql);
		return FALSE;
	}
	dessql = mysql_real_connect(dessql, deshost.hostip, deshost.username,deshost.pwd, deshost.store, atoi(deshost.port), NULL, 0);
	if (dessql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "连接目标数据库错误！%d,ip:%s %d", mysql_error(dessql), deshost.hostip,deshost.port);
		return FALSE;

	}

	scsql = mysql_init((MYSQL*)0);
	scsql = mysql_real_connect(scsql, schost.hostip, schost.username, schost.pwd, schost.store, atoi(schost.port), NULL, 0);
	if (scsql == NULL)
	{
		Log(Logs::Error, Logs::MysqlErro, "连接源数据库错误！%d", mysql_error(scsql));
		mysql_close(scsql);
		return FALSE;
	}

	if ((mysql_set_character_set(dessql, "GBK") != 0)&&(mysql_set_character_set(scsql, "GBK") != 0))

	{

		Log(Logs::General, Logs::MysqlErro, "设置GBK字符集错误！");

		return FALSE;

	}

	if (mysql_select_db(dessql, deshost.store))
	{
		Log(Logs::General, Logs::MysqlErro, "设置目标库错误！");
		return FALSE;
	}
	dessql->reconnect = 1;

	if (mysql_select_db(scsql, schost.store))
	{
		Log(Logs::General, Logs::MysqlErro, "设置源库错误！");
		return FALSE;
	}
	scsql->reconnect = 1;

	Log(Logs::General, Logs::Normal, "数据库连接完成！");




	return TRUE;
}

bool Ceqmyslq::runSQLCommand(CString sql)
{
	if ((!dessql->reconnect)&&(scsql->reconnect))
	{//    没有连接到服务器    
		Log(Logs::General, Logs::MysqlErro, "没有数据库连接！");
		return false;
	}
	if (sql.IsEmpty())
	{//    SQL语句为空    
		Log(Logs::General, Logs::MysqlErro, "SQL命令为空！");
		return false;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;
	
	unsigned int i, j = 0;

	i = mysql_real_query(dessql, sql, (unsigned int)strlen(sql));//    执行查询    
	if (i < 0)
	{
		Log(Logs::General, Logs::MysqlErro, "MYSQL查询指令错误：%s",mysql_error(dessql));
		return false;
	}

	res = mysql_store_result(dessql);
	if (res == NULL)
	{
		Log(Logs::General, Logs::MysqlErro, "MYSQL查询指令错误：%s", mysql_error(dessql));
		return false;
	}


	while ((row = mysql_fetch_row(res)))
	{//    遍历结果集    
		objectValue.clear();
		//j = 1;
		for (j = 0; j < mysql_num_fields(res); j++)
		{
			if (row[j] == NULL)
				objectValue.push_back("");
			else
			objectValue.push_back(row[j]);
		}
		//Log(Logs::General, Logs::Normal, "MYSQL：%s", objectValue.back());

		resultList.push_back(objectValue);
	}
	mysql_free_result(res);   //free result after you get the result    

//	for(int i=0;i<resultList.size();++i)
//		Log(Logs::General, Logs::Normal, "MYSQL：%s", resultList[i][0].c_str());

	return true;
}

unsigned int Ceqmyslq::insert(std::string sql)
{
	if ((!dessql->reconnect) && (scsql->reconnect)) {
		Log(Logs::General, Logs::MysqlErro, "没有数据库连接！");
		return -1;
	}
	if (sql.empty()) {
		Log(Logs::General, Logs::MysqlErro, "SQL命令为空！");
		return -1;
	}
	int rows = -1;
	int res = mysql_query(dessql, sql.c_str());
	if (res >= 0) {
		// 返回受影响的行数
		rows = (int)mysql_affected_rows(dessql);
		Log(Logs::General, Logs::MysqlErro, "返回受影响行数：%d",row);
		return rows;
	}
	else {
		Log(Logs::General, Logs::MysqlErro, "SQL插入命令错误！");
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

//关闭数据库连接
void Ceqmyslq::Close()
{
	mysql_close(dessql);
	mysql_close(scsql);
	Log(Logs::General, Logs::Normal, "已断开数据库连接！");
}
