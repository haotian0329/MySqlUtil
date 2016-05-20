# coding: UTF-8
import MySQLdb

#cluster集群sql参数
CLUSTER_IP_ADDR='192.168.0.51'
CLUSTER_USER_NAME='root'
CLUSTER_PASSWORD='111111'
CLUSTER_PORT=3306
CLUSTER_DB_NAME='security'


#cluster2集群sql参数
CLUSTER2_IP_ADDR='192.168.0.51'
CLUSTER2_USER_NAME='root'
CLUSTER2_PASSWORD='111111'
CLUSTER2_PORT=3306
CLUSTER2_DB_NAME='security'



#sqlType为0表示cluster集群数据库，sqlType为1表示cluster2集群数据库
class SqlUtil():
    def __init__(self,sqlType):
        self.sqlType=sqlType
        if self.sqlType==0:
            self.ipAddr=CLUSTER_IP_ADDR
	    self.userName=CLUSTER_USER_NAME
	    self.passwd=CLUSTER_PASSWORD
	    self.port=CLUSTER_PORT
	    self.dbName=CLUSTER_DB_NAME
	elif self.sqlType==1:
            self.ipAddr=CLUSTER2_IP_ADDR
	    self.userName=CLUSTER2_USER_NAME
	    self.passwd=CLUSTER2_PASSWORD
	    self.port=CLUSTER2_PORT
	    self.dbName=CLUSTER2_DB_NAME

    def connect(self):
        try:
            self.conn=MySQLdb.connect(host=self.ipAddr,user=self.userName,passwd=self.passwd,port = self.port)
            self.conn.select_db(self.dbName)
        except MySQLdb.Error,e:
            print "Mysql Error %s" %(e)

    def disconnect(self):
        try:
           self.conn.close();
        except MySQLdb.Error,e:
            print "Mysql Error %s" %(e)

    def getDataFromSql(self,sqlStr):
        try:
            cur=self.conn.cursor()
            cur.execute(sqlStr)
            results=cur.fetchall()
            cur.close()
        except MySQLdb.Error,e:
            print "Mysql Error %s" %(e)
        return results

    def insertDataToSql(self,sqlStr):
        try:
            cur=self.conn.cursor()
            cur.execute(sqlStr)
            self.conn.commit()
            cur.close()
        except MySQLdb.Error,e:
            print "Mysql Error %s" %(e)

    def getCursor(self):
        try:
            cur=self.conn.cursor()
        except MySQLdb.Error,e:
          print "Mysql Error %s" %(e)
        return cur
