/*
 * mysqlUtil.h
 *
 *  Created on: Jun 21, 2016
 *      Author: haotian.wh
 */

#ifndef INCLUDE_MYSQLUTIL_H_
#define INCLUDE_MYSQLUTIL_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HOST "100.81.152.47"
#define USERNAME "root"
#define PASSWORD ""
#define DATEBASE "test"
#define PORT 3306

#define UNIX_SOCKET NULL

/*
	default 0
	CLIENT_FOUND_ROWS
	CLIENT_NO_SCHEMA
	CLIENT_COMPRESS
	CLIENT_ODBC
*/
#define CLIENT_FLAG 0

#define DEBUG 0

typedef void (*MYSQLOPERATEFUNC)(void * pdata, void * param);

void connect_mysql(MYSQL * conn);

void disconnect_mysql(MYSQL * conn);

void exec_mysql_str(MYSQL * conn,const char* sqlStr);

void get_data_and_operate(MYSQL * conn, const char* sqlStr, MYSQLOPERATEFUNC mysqlOperateFunc, void * param);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MYSQLUTIL_H_ */
