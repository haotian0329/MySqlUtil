/*
 * mysqlUtil.c
 *
 *  Created on: Jun 21, 2016
 *      Author: haotian.wh
 */

#include "mysqlUtil.h"

/**
 * connect to mysql server
 */
void connect_mysql(MYSQL * conn){
	//initialize mysql connction
	mysql_init(conn);
	if(conn==NULL){
		printf("Error mysql_init:%s !!!\n",mysql_error(conn));
		perror("Error mysql_init");
		exit(-1);
	}else if(DEBUG){
		printf("mysql_init successfully!!!\n");
	}
	//connect to mysql server
	if(mysql_real_connect(conn,HOST,USERNAME,PASSWORD,DATEBASE,PORT,UNIX_SOCKET,CLIENT_FLAG)!=NULL){
		if(DEBUG) printf("mysql_real_connect successfully!!!\n");
		//choose database
		if(mysql_select_db(conn,DATEBASE)==0){
			if(DEBUG) printf("mysql_select_db successfully!!!\n");
			//set auto reconnect
			conn->reconnect=1;
		}else{
			perror("mysql_select_db");
			exit(-1);
		}
	}else{
		printf("Error mysql_real_connect:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}

/**
 * disconnect to mysql server
 */
void disconnect_mysql(MYSQL * conn){
	mysql_close(conn);
	if(DEBUG) printf("mysql_close successfully!!!\n");
}


void exec_mysql_str(MYSQL * conn,const char* sqlStr){
	if(DEBUG) printf("sql exec cmd:%s\n",sqlStr);
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}

/**
 * An example for get data
 */
void get_data_and_operate(MYSQL * conn, const char* sqlStr, MYSQLOPERATEFUNC mysqlOperateFunc, void * param){
	if(DEBUG) printf("sql query cmd:%s\n",sqlStr);
	//exec query
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
		//store result set
		MYSQL_RES *res=mysql_store_result(conn);
		if(res==NULL){
			printf("Error mysql_store_result:%s !!!\n",mysql_error(conn));
			exit(-1);
		}else{
			if(DEBUG) printf("mysql_store_result successfully!!!\n");
		}
		MYSQL_ROW row;
		int index;
		while((row=mysql_fetch_row(res))){
			for(index=0;index<(int)mysql_num_fields(res);index++){
				if(mysqlOperateFunc!=NULL)//use callback func
					mysqlOperateFunc(row[index],param);
			}
		}
		//free result set
		mysql_free_result(res);
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}
