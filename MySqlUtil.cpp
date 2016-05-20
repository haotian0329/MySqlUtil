#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>// 进行网络编程需要winsock2.h 
#include <mysql.h>
#pragma comment(lib,"libmysql")

//主机名或IP地址,如果host是NULL或"localhost"，是到本地主机的一个连接
#define HOST "localhost"
//登录用户名
#define USERNAME "root"
//登录密码
#define PASSWORD "123456"
//数据库名称
#define DATEBASE "test"
//如果port不是0,对应MYSQL服务器的TCP服务端口
#define PORT 3306
//如果不是NULL，字符串指定套接字或应该被使用的命名管道
#define UNIX_SOCKET NULL
/*
	默认是0
	CLIENT_FOUND_ROWS --返回找到的(匹配的)行数,不是受到影响的行数
	CLIENT_NO_SCHEMA  --不允许db_name.tbl_name.col_name语法
	CLIENT_COMPRESS   --使用压缩协议
	CLIENT_ODBC       --客户是一个ODBC客户
*/
#define CLIENT_FLAG 0
//是否打印调试信息
#define DEBUG 1

/************************************************************************/
/* 连接数据库                                                           */
/************************************************************************/
void connect(MYSQL * conn){
	//初始化mysql连接
	mysql_init(conn);
	if(conn==NULL){
		printf("Error mysql_init:%s !!!\n",mysql_error(conn));
		perror("Error mysql_init");
		exit(-1);
	}else if(DEBUG){
		printf("mysql_init successfully!!!\n");
	}
	//连接数据库
	if(mysql_real_connect(conn,HOST,USERNAME,PASSWORD,DATEBASE,PORT,UNIX_SOCKET,CLIENT_FLAG)!=NULL){
		if(DEBUG) printf("mysql_real_connect successfully!!!\n");
		//选择默认缺省的数据库
		if(mysql_select_db(conn,DATEBASE)==0){
			if(DEBUG) printf("mysql_select_db successfully!!!\n");
			//设置自动重连
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

/************************************************************************/
/* 断开数据库                                                           */
/************************************************************************/
void disconnect(MYSQL * conn){
	mysql_close(conn);
	if(DEBUG) printf("mysql_close successfully!!!\n");
}


void execSql(MYSQL * conn,const char* sqlStr){
	if(DEBUG) printf("sql exec cmd:%s\n",sqlStr);
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);
	}
}

/************************************************************************/
/* 获取数据（简单打印数据，调用者根据实际情况改写存储）                 */
/************************************************************************/
void getData(MYSQL * conn, const char* sqlStr){
	if(DEBUG) printf("sql query cmd:%s\n",sqlStr);
	//执行查询语句
	if(mysql_real_query(conn,sqlStr,(unsigned long)strlen(sqlStr))==0){
		if(DEBUG) printf("mysql_real_query successfully!!!\n");
		//存储结果集
		MYSQL_RES *res=mysql_store_result(conn);
		if(res==NULL){
			printf("Error mysql_store_result:%s !!!\n",mysql_error(conn));
			exit(-1);
		}else{
			if(DEBUG) printf("mysql_store_result successfully!!!\n");
		}
		MYSQL_ROW row;
		int index;
		while((row=mysql_fetch_row(res))){//检索行
			for(index=0;index<(int)mysql_num_fields(res);index++){
				printf("%s	",row[index]);
			}
			printf("*****************************\n");
		}
		//释放结果集 
		mysql_free_result(res);
	}else{
		printf("Error mysql_real_query:%s !!!\n",mysql_error(conn));
		exit(-1);		
	}
}

int main(){
	MYSQL conn;//不能声明成MYSQL *conn；
	connect(&conn);
	//const char* str="insert into student values('3','lucy',19);";
	//execSql(&conn,str);
	const char* str="select * from student;";
	getData(&conn,str);
	disconnect(&conn);
	getchar();
	return 0;
}