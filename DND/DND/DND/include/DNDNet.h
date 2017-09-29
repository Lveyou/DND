//////////////////////////////////////////////////////////////////////////
//name:		DNDNet
//author:	Lveyou
//data:		17-08-21

//other:
//17-08-21: ���紫��ģ�飬ʹ��Winsock2�����߳�����ģ�͡�
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
	////////////////////////////////����hash_code�ж����͵Ŀ���///////////////////////////////
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
			FAIL = -1,	//����
			INIT = 0,	//��ʼ��
			LINK,		//������
			FREE,		//����
			SEND,		//������
			RECV		//������

		};
		//������ָ����������ַ�Ͷ˿�����
		virtual void Connect(const String& ip, const int port) = 0;
		//�Ͽ����Ӳ��ͷ���Դ
		virtual void DisConnect() = 0;
		//����һ����Ϣ
		virtual void Send(const NetMsg& msg) = 0;
		//ȡһ����Ϣ���д���
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
		//��ʼ����ָ���˿�
		virtual void StartListen(UINT16 port) = 0;
		//ͨ��ID���IP
		virtual String GetClientIP(UINT32 id) = 0;
		//��ȡClient����
		virtual unsigned GetClientNum() = 0;
		//����Client����
		virtual void GetAllClient(ClientUser* users) = 0;
		//������Ϣ�ص�
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) = 0;
		//���߻ص�
		virtual void SetProcBegin(void(*func)(UINT32 id)) = 0;
		//���߻ص�
		virtual void SetProcEnd(void(*func)(UINT32 id)) = 0;
		//���͸�ָ��Client��Ϣ
		virtual void Send(UINT32 id, NetMsg msg) = 0;
		//����ͷ����ʽ�ַ���
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