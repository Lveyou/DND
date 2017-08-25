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
using namespace std;

namespace DND
{

	class Client_imp : public Client, public Thread
	{
	public:
		//尝试向指定服务器地址和端口连接
		virtual void Connect(const String& ip, const int port) override;
		//发送一个消息
		virtual void Send(const NetMsg& msg) override;
		//取一个消息进行处理
		virtual NetMsg Recv() override;
		virtual State GetState() override;

		void _run();

		list<NetMsg> m_sends;
		list<NetMsg> m_recvs;
		//volatile unsigned m_lock_send;//
		volatile unsigned m_lock_recv;//
		//关于线程同步：
		//客户端：
		//线程在读m_sends的时候，用户不能Send写入
		volatile unsigned m_lock_send;//
		//用户在调用Send的时候，增加m_sends的长度，增加完之后，线程才读取长度
		//用户在调用Revc的时候，线程可能在添加数据，用户需要线程确保添加完成才读取
		//服务器： 接收一次发送一次

		String m_server_ip;
		int m_port;
		SOCKET m_socket;
		String m_buffer_send;
		String m_buffer_recv;



		volatile State m_state;
		~Client_imp();
	};

	class Server_imp : public Server
	{
	public:
		//开始监听指定端口
		virtual void StartListen(const int port) override;
		//通过ID获得IP
		virtual String GetIP(unsigned id) override;
		//获取Client数量
		virtual unsigned GetClientNum() override;
		//返回Client数组
		virtual void GetAllClient(ClientUser* users) override;
		//设置消息回调
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) override;
		//断线回调
		virtual void SetProcEnd(void(*func)(UINT32 id)) override;
		//发送给指定Client消息
		virtual void Send(UINT32 id, NetMsg msg) override;
	};

}

#endif