#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

#include <muduo/net/TcpServer.h>
#include <unordered_map>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

// 表示处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService *instance();

    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 创建群聊业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 加入群聊业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 处理注销业务
    void logout(const TcpConnectionPtr &conn, json &js, Timestamp);

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);

    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);

    // 从redis队列中获取订阅消息
    void handlerRedisSubscribeMessage(int,string);

private:
    ChatService();

    // 存储消息id和其对应的消息函数
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 数据操作类对象
    UserModel _userModel;
    offlineMsgModel _offlineMsgModel;
    FriendModel _friendmodel;
    GroupModel _groupModel;

    // 存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // redis对象
    Redis _redis;
};

#endif // !CHATSERVICE_H