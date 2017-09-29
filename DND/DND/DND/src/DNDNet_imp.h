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
		//������ָ����������ַ�Ͷ˿�����
		virtual void Connect(const String& ip, const int port) override;
		//�Ͽ����Ӳ��ͷ���Դ
		virtual void DisConnect() override;
		//����һ����Ϣ
		virtual void Send(const NetMsg& msg) override;
		//ȡһ����Ϣ���д���
		virtual NetMsg Recv() override;
		//��ȡ����״̬
		virtual UINT32 GetState() override;

		void _run();

		list<NetMsg> m_sends;
		list<NetMsg> m_recvs;
	
		//�����߳�ͬ����
		//�ͻ��ˣ�
		//�߳��ڶ�m_sends��ʱ���û�����Sendд��
		//�û��ڵ���Send��ʱ������m_sends�ĳ��ȣ�������֮���̲߳Ŷ�ȡ����
		//�û��ڵ���Revc��ʱ���߳̿�����������ݣ��û���Ҫ�߳�ȷ�������ɲŶ�ȡ
		//�������� ����һ�η���һ��
		volatile UINT32 m_lock_recv;
		volatile UINT32 m_lock_send;

		String m_server_ip;//��������ַ
		int m_port;//�������˿�
		SOCKET m_socket;//�Լ���SOCKET

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
		NetMsg m_msg;//���ͻ�������Server����new���Լ�����delete
		volatile UINT32 m_sending;
	};

	class ClientInfo
	{
	public:
		SOCKET socket;
		String ip;
		ServerRecv thread;//���thread���ܴ�����ջ��
		ClientInfo();
	};

	class Server_imp : public Server, public Thread
	{
	public:
		//��ʼ����ָ���˿�
		virtual void StartListen(UINT16 port) override;
		//ͨ��ID���IP
		virtual String GetClientIP(UINT32 id) override;
		//��ȡClient����
		virtual unsigned GetClientNum() override;
		//����Client����
		virtual void GetAllClient(ClientUser* users) override;
		//������Ϣ�ص�
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) override;
		//���߻ص�
		virtual void SetProcBegin(void(*func)(UINT32 id)) override;
		//���߻ص�
		virtual void SetProcEnd(void(*func)(UINT32 id)) override;
		//���͸�ָ��Client��Ϣ
		virtual void Send(UINT32 id, NetMsg msg) override;
		virtual String GetClientInfo(UINT32 id) override;

		static void (*m_proc_func)(UINT32 id, NetMsg str);
		static void(*m_proc_func_begin)(UINT32 id);
		static void(*m_proc_func_end)(UINT32 id);

		void _run();

		int m_port;
		SOCKET m_socket;

		//����ʱ���ܶ�ȡ����
		volatile UINT32 m_lock;

		map<UINT32, ClientInfo*> m_clients;
		void _add_client(ClientInfo* info);

		~Server_imp();
	};

}

#endif