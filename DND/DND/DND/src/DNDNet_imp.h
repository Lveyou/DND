//////////////////////////////////////////////////////////////////////////
//name:		DNDNet_imp
//author:	Lveyou
//data:		17-08-25

//other:

//////////////////////////////////////////////////////////////////////////

#ifndef _DND_NET_IMP_H_
#define _DND_NET_IMP_H_

#include "DNDNet.h"
#include "DNDThread.h"


#include <WinSock2.h>


#include <list>
#include <map>
using namespace std;

namespace DND
{

	class Client_imp : public Client, public Thread
	{
	public:
		//尝试向指定服务器地址和端口连接
		virtual void Connect(const String& ip, const int port) override;
		//断开连接并释放资源
		virtual void DisConnect() override;
		//发送一个消息
		virtual void Send(const NetMsg& msg) override;
		//取一个消息进行处理
		virtual NetMsg Recv() override;
		//获取网络状态
		virtual UINT32 GetState() override;

		void _run();

		list<NetMsg> m_sends;
		list<NetMsg> m_recvs;
	
		//关于线程同步：
		//客户端：
		//线程在读m_sends的时候，用户不能Send写入
		//用户在调用Send的时候，增加m_sends的长度，增加完之后，线程才读取长度
		//用户在调用Revc的时候，线程可能在添加数据，用户需要线程确保添加完成才读取
		//服务器： 接收一次发送一次
		volatile UINT32 m_lock_recv;
		volatile UINT32 m_lock_send;

		String m_server_ip;//服务器地址
		int m_port;//服务器端口
		SOCKET m_socket;//自己的SOCKET

		volatile UINT32 m_state;
		bool m_end;
		~Client_imp();
	};

	class ServerRecv : public Thread
	{
	public:
		void Init(SOCKET socket, UINT32 id);
		void _run();
		SOCKET m_socket;
		UINT32 m_id;
		NetMsg m_msg;//发送缓冲区，Server负责new，自己负责delete
		volatile UINT32 m_sending;
	};

	class ClientInfo
	{
	public:
		SOCKET socket;
		String ip;
		ServerRecv thread;//这个thread不能创建在栈上
		ClientInfo();
	};

	class Server_imp : public Server, public Thread
	{
	public:
		//开始监听指定端口
		virtual void StartListen(UINT16 port) override;
		//通过ID获得IP
		virtual String GetClientIP(UINT32 id) override;
		//获取Client数量
		virtual unsigned GetClientNum() override;
		//返回Client数组
		virtual void GetAllClient(ClientUser* users) override;
		//设置消息回调
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) override;
		//连线回调
		virtual void SetProcBegin(void(*func)(UINT32 id)) override;
		//断线回调
		virtual void SetProcEnd(void(*func)(UINT32 id)) override;
		//发送给指定Client消息
		virtual void Send(UINT32 id, NetMsg msg) override;
		virtual String GetClientInfo(UINT32 id) override;

		static void (*m_proc_func)(UINT32 id, NetMsg str);
		static void(*m_proc_func_begin)(UINT32 id);
		static void(*m_proc_func_end)(UINT32 id);

		void _run();

		int m_port;
		SOCKET m_socket;

		//加锁时不能读取数据
		volatile UINT32 m_lock;

		map<UINT32, ClientInfo*> m_clients;
		void _add_client(ClientInfo* info);

		~Server_imp();
	};

}

#endif