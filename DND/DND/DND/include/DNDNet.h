//////////////////////////////////////////////////////////////////////////
//name:		DNDNet
//author:	Lveyou
//data:		17-08-21

//other:
//17-08-21: 网络传输模块，使用Winsock2，多线程阻塞模型。
//////////////////////////////////////////////////////////////////////////

#ifndef _DND_NET_H_
#define _DND_NET_H_

#include "DNDDLL.h"
#include "DNDString.h"
#include <typeinfo>

namespace DND
{
	class DLL_API NetMsg
	{
	public:
		template<typename T>
		void Build(T* p)
		{
			_type = typeid(p).hash_code();
			_data = (void*)p;
			_size = sizeof(T);
		}
	private:
		UINT32 _type;//4
		UINT32 _size;//4
		void* _data;
	};


	class DLL_API Client
	{
	public:
		enum State
		{
			FAIL = -1,	//断线
			INIT = 0,	//初始化
			LINK,		//连接中
			FREE,		//空闲
			SEND,		//发送中
			RECV		//接收中

		};
		//尝试向指定服务器地址和端口连接
		virtual void Connect(const String& ip, const int port) = 0;
		//发送一个消息
		virtual void Send(const NetMsg& msg) = 0;
		//取一个消息进行处理
		virtual NetMsg Recv() = 0;
		virtual State GetState() = 0;
	};

	struct ClientUser
	{
		UINT32 id;
		String ip;
	};

	class DLL_API Server
	{
	public:
		//开始监听指定端口
		virtual void StartListen(const int port) = 0;
		//通过ID获得IP
		virtual String GetIP(unsigned id) = 0;
		//获取Client数量
		virtual unsigned GetClientNum() = 0;
		//返回Client数组
		virtual void GetAllClient(ClientUser* users) = 0;
		//设置消息回调
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) = 0;
		//断线回调
		virtual void SetProcEnd(void(*func)(UINT32 id)) = 0;
		//发送给指定Client消息
		virtual void Send(UINT32 id, NetMsg msg) = 0;
	};

	class DLL_API Net
	{
	public:
		static Client* GetClient();
		static Server* GetServer();
	};

}

#endif