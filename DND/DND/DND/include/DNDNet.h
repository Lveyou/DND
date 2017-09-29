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

#define DND_SERVER_MSG_HEAD() \
	UINT32 type = msg.GetType();\
	if(type == 0)\
		return;

#define DND_SERVER_ON_MSG(name) \
	if(type == GetClassType<name>())\
		{OnMsg_##name(id, msg.UnBuild<name>());return;}

#define DND_CLIENT_MSG_HEAD() \
	UINT32 type = msg.GetType();\
	if(type == 0)\
		return;

#define DND_CLIENT_ON_MSG(name) \
	if(type == GetClassType<name>())\
		{OnMsg_##name(msg.UnBuild<name>());return;}
	


namespace DND
{
	////////////////////////////////降低hash_code判断类型的开销///////////////////////////////
	template<typename T>
	class ClassType
	{
	public:
		UINT32 _code;
		ClassType()
		{
			_code = typeid(T).hash_code();
		}
	};

	template<typename T>
	inline UINT32 GetClassType()
	{
		static ClassType<T> type;
		return type._code;
	}
	//////////////////////////////////////////////////////////////////////////
	class DLL_API NetMsg
	{
		friend class Client_imp;
		friend class Server_imp;
		friend class ServerRecv;
	public:
		template<typename T>
		static NetMsg Build(T* p)
		{
			NetMsg ret;
			ret._type = GetClassType<T>();
			ret._data = (void*)p;
			ret._size = sizeof(T);
			return ret;
		}
		UINT32 GetType()
		{
			return _type;
		}
		template<typename T>
		T* UnBuild()
		{
			dnd_assert(_type == GetClassType<T>(), ERROR_00050);
			return (T*)_data;
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
		//断开连接并释放资源
		virtual void DisConnect() = 0;
		//发送一个消息
		virtual void Send(const NetMsg& msg) = 0;
		//取一个消息进行处理
		virtual NetMsg Recv() = 0;
		virtual UINT32 GetState() = 0;
		virtual ~Client();
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
		virtual void StartListen(UINT16 port) = 0;
		//通过ID获得IP
		virtual String GetClientIP(UINT32 id) = 0;
		//获取Client数量
		virtual unsigned GetClientNum() = 0;
		//返回Client数组
		virtual void GetAllClient(ClientUser* users) = 0;
		//设置消息回调
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) = 0;
		//连线回调
		virtual void SetProcBegin(void(*func)(UINT32 id)) = 0;
		//断线回调
		virtual void SetProcEnd(void(*func)(UINT32 id)) = 0;
		//发送给指定Client消息
		virtual void Send(UINT32 id, NetMsg msg) = 0;
		//返回头部格式字符串
		virtual String GetClientInfo(UINT32 id) = 0;
	};

	class DLL_API Net
	{
	public:
		static Client* GetClient();
		static Server* GetServer();
		static BYTE* GetPhysicalAddress();
	private:
		static int _bInit;

	};

	

}

#endif