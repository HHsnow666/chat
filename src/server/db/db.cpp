#include "db.hpp"
#include <muduo/base/Logging.h>
#include <iostream>
using namespace muduo;

// 数据库配置信息
static string server = "127.0.0.1";
static string user = "root";
static string password = "12345678";
static string dbname = "chat";

// 数据库操作类
MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}

// 释放数据库连接资源
MySQL::~MySQL()
{
    if (_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // C和C++的默认字符是ascii，需要转换成gbk
        mysql_query(_conn, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql fail!";
    }
    return p;
}

// 更新操作
bool MySQL::update(string sql)
{
    int res =mysql_real_query(_conn, sql.c_str(), strlen(sql.c_str()));
    if (res != 0)
    {
        cout << "------update----------"<<res<<"--------------------"<<endl;
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }    
    return true;
}

// 查询操作
MYSQL_RES *MySQL::query(string sql)
{
    int res = mysql_real_query(_conn, sql.c_str(), strlen(sql.c_str()));
    if (res != 0)
    {
        cout << "------query----------"<<res<<"--------------------" << endl;
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }    
    return mysql_use_result(_conn);
}

// 返回连接状态
MYSQL *MySQL::getConnection()
{
    return _conn;
}
