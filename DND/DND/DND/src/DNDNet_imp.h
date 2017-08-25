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
		//������ָ����������ַ�Ͷ˿�����
		virtual void Connect(const String& ip, const int port) override;
		//����һ����Ϣ
		virtual void Send(const NetMsg& msg) override;
		//ȡһ����Ϣ���д���
		virtual NetMsg Recv() override;
		virtual State GetState() override;

		void _run();

		list<NetMsg> m_sends;
		list<NetMsg> m_recvs;
		//volatile unsigned m_lock_send;//
		volatile unsigned m_lock_recv;//
		//�����߳�ͬ����
		//�ͻ��ˣ�
		//�߳��ڶ�m_sends��ʱ���û�����Sendд��
		volatile unsigned m_lock_send;//
		//�û��ڵ���Send��ʱ������m_sends�ĳ��ȣ�������֮���̲߳Ŷ�ȡ����
		//�û��ڵ���Revc��ʱ���߳̿�����������ݣ��û���Ҫ�߳�ȷ�������ɲŶ�ȡ
		//�������� ����һ�η���һ��

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
		//��ʼ����ָ���˿�
		virtual void StartListen(const int port) override;
		//ͨ��ID���IP
		virtual String GetIP(unsigned id) override;
		//��ȡClient����
		virtual unsigned GetClientNum() override;
		//����Client����
		virtual void GetAllClient(ClientUser* users) override;
		//������Ϣ�ص�
		virtual void SetProc(void (*func)(UINT32 id, NetMsg msg)) override;
		//���߻ص�
		virtual void SetProcEnd(void(*func)(UINT32 id)) override;
		//���͸�ָ��Client��Ϣ
		virtual void Send(UINT32 id, NetMsg msg) override;
	};

}

#endif