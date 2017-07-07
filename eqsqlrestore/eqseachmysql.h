#include "mysql.h"
#ifndef __DBCTRL_H__

#endif __DBCTRL_H__

#include <winsock.h>
#include <iostream>
#include <string>
#include <mysql.h>

#pragma comment(lib,"libmysql.lib")
#pragma once
using namespace std;

class eqseachmysql
{
public:
	eqseachmysql();
	~eqseachmysql();
private:
	MYSQL mydata;
};

