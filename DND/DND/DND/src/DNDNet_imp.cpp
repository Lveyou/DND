#include "DNDNet_imp.h"
#include "DNDDebug.h"


namespace DND
{
	const UINT32 BUFFER_SIZE = 8192;
	void(*Server_imp::m_proc_func)(UINT32 id, NetMsg msg) = NULL;
	void(*Server_imp::m_proc_func_end)(UINT32 id) = NULL;
	void(*Server_imp::m_proc_func_begin)(UINT32 id) = NULL;

	//static list<Client_imp*> listClient;
	Client* Net::GetClient()
	{
		if(_bInit == 0)
		{
			WSADATA wsaData;
			WORD scokVersion = MAKEWORD(2, 2);
			dnd_assert(!WSAStartup(scokVersion, &wsaData), ERROR_00049);
			debug_notice(L"DND: Net init ok!");
			_bInit = 1;
		}
			
		Client_imp* c = new Client_imp;
		return c;
	}

	Server* Net::GetServer()
	{
		if(_bInit == 0)
		{
			WSADATA wsaData;
			WORD scokVersion = MAKEWORD(2, 2);
			dnd_assert(!WSAStartup(scokVersion, &wsaData), ERROR_00049);
			debug_notice(L"DND: Net init ok!");
			_bInit = 1;
		}
		//Serverֻ��һ��
		static Server_imp* s = new Server_imp;
		return s;
	}

	int Net::_bInit = 0;

	void Client_imp::Connect(const String& ip, const int port)
	{
		m_server_ip = ip;
		m_port = port;
		m_lock_send = 0;
		m_lock_recv = 0;
		m_end = 0;
		Start();
	}

	void Client_imp::DisConnect()
	{
		m_end = true;
		closesocket(m_socket);
	}

	void Client_imp::Send(const NetMsg& msg)
	{
		//���︴��msg��buffer�����ڶ��ϣ��ȷ��͸����������ͷš�
		NetMsg temp = msg;
		temp._data = new BYTE[msg._size];
		memcpy(temp._data, msg._data, msg._size);
		m_sends.push_back(temp);
	}

	NetMsg Client_imp::Recv()
	{
		if(!m_recvs.size())
		{
			NetMsg ret;
			ret._type = 0;
			return ret;
		}
		NetMsg ret = m_recvs.front();
		m_recvs.pop_front();
		return ret;
	}

	UINT32 Client_imp::GetState()
	{
		return m_state;
	}

	void Client_imp::_run()
	{
		UINT32 state = 0;//��ʼ
		InterlockedExchange(&m_state, state);

		char buffer[BUFFER_SIZE];
re2://��������
		if (m_end)
			return;
		//�����׽���
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			debug_err(L"DND:Client �����׽���ʧ�ܡ�");
			return;
		}

		//���ӷ�����
		SOCKADDR_IN server_ip;
		server_ip.sin_family = AF_INET;
		m_server_ip.GetMultiByteStr(buffer, BUFFER_SIZE);
		//inet_pton(AF_INET, buffer, (void*)&server_ip);
		server_ip.sin_addr.s_addr = inet_addr(buffer);
		server_ip.sin_port = htons((short)m_port);

re:	
		if (m_end)
			return;
		state = 1;//������
		InterlockedExchange(&m_state, state);
		int ret = connect(m_socket, (LPSOCKADDR)&server_ip, sizeof(server_ip));
		if (ret == SOCKET_ERROR)
		{
			state = -1;//ʧ��
			InterlockedExchange(&m_state, state);
			debug_warn(L"DND: Clinet���ӷ�����ʧ�ܡ�");
			Sleep(3000);//3�������
			
			goto re;
			
		}

		debug_notice(L"DND: Clinet���ӷ������ɹ���");

		//�������ѭ��
		while (!m_end)
		{
			//���û����Ϣ���� ����sleep�߳�
			if (m_sends.size() == 0)
			{
				state = 2;//����
				InterlockedExchange(&m_state, state);
				Sleep(100);
				continue;
			}

			unsigned lock_send = 1;//��ֹSend����д��
			InterlockedExchange(&m_lock_send, lock_send);

			//�Ӷ���ȡ��һ����Ϣ
			NetMsg msg = m_sends.front();
			
			lock_send = 0;//
			InterlockedExchange(&m_lock_send, lock_send);

			state = 3;//����

			InterlockedExchange(&m_state, state);
			memcpy(buffer, &msg._type, sizeof(msg._type));
			memcpy(buffer + sizeof(msg._type), &msg._size, sizeof(msg._size));
			memcpy(buffer + sizeof(msg._type) + sizeof(msg._size),
				msg._data, msg._size);

			ret = send(m_socket, buffer, sizeof(msg._type) + sizeof(msg._size) + msg._size, 0);


			if (ret == SOCKET_ERROR)
			{
				debug_err(L"DND:Clinet ��������ʧ�ܡ�");
				closesocket(m_socket);
				goto re2;
			}
			//�ɹ�����֮���ͷŶ��ڴ棬���Ƴ�msg
			m_sends.pop_front();
			delete[] msg._data;

			state = 4;//����
			InterlockedExchange(&m_state, state);
			ret = recv(m_socket, buffer, BUFFER_SIZE, 0);


			if (ret == SOCKET_ERROR)
			{
				debug_err(L"DND:Clinet ��������ʧ�ܡ�");
				closesocket(m_socket);
				goto re2;
			}

			//�����յ�����Ϣ����һ��NetMsg���û�Unbuild���ͷŶ��ڴ�
			NetMsg msg2;
			memcpy(&msg2._type, buffer, sizeof(msg2._type));
			memcpy(&msg2._size, buffer + sizeof(msg2._type), sizeof(msg2._size));
			msg2._data = new BYTE[msg2._size];
			memcpy(msg2._data, buffer + sizeof(msg2._type) + sizeof(msg2._size), msg2._size);

			UINT32 lock = 1;
			InterlockedExchange(&m_lock_recv, lock);

			m_recvs.push_back(msg2);

			lock = 0;
			InterlockedExchange(&m_lock_recv, lock);

			state = 2;//���У���ʱ��֤bufferrecv����ȷ��
			InterlockedExchange(&m_state, state);
		}
	}

	Client::~Client()
	{

	}


	Client_imp::~Client_imp()
	{
		closesocket(m_socket);
		
	}

	ClientInfo::ClientInfo()
	{
		//ip = L"";
	}



	void ServerRecv::Init(SOCKET socket, UINT32 id)
	{
		m_id = id;
		m_socket = socket;

		
		Start();
	}

	void ServerRecv::_run()
	{
		char buffer[BUFFER_SIZE];
		while (true)
		{
			int ret = recv(m_socket, buffer, BUFFER_SIZE, 0);

			if (ret == SOCKET_ERROR)
			{
end:
				//��ת������˵�����Է��Ѿ��Ͽ�������
				debug_warn(((Server_imp*)(Net::GetServer()))->GetClientInfo(m_id) + L"�Ͽ�������");
				if (Server_imp::m_proc_func_end)
					Server_imp::m_proc_func_end(m_id);
				//�����̣߳�ɾ��ClientInfo
				closesocket(m_socket);
				((Server_imp*)(Net::GetServer()))->m_clients.erase(m_id);
				//m_socket = INVALID_SOCKET;
				return;
			}
			else
			{
				//���뵽�����˵���ͻ�������������
				unsigned lock_send = 0;
				InterlockedExchange(&m_sending, lock_send);

				//���ﴦ��ͻ��˷�������Ϣ
				if (Server_imp::m_proc_func)
				{
					//�����Լ�����new����Ϣ�����������delete
					NetMsg msg2;
					memcpy(&msg2._type, buffer, sizeof(msg2._type));
					memcpy(&msg2._size, buffer + sizeof(msg2._type), sizeof(msg2._size));
					//���͵�buffer����ֱ�ӶϿ�����
					if (msg2._size > BUFFER_SIZE - sizeof(msg2._type) - sizeof(msg2._size))
					{
						goto end;
					}
					msg2._data = new BYTE[msg2._size];
					memcpy(msg2._data, buffer + sizeof(msg2._type) + sizeof(msg2._size), msg2._size);

					Server_imp::m_proc_func(m_id, msg2);

					delete[] msg2._data;
				}

				//�����ȴ� m_msg�����
				while (m_sending == 0)//û��������Ҫ����
				{
					Sleep(100);
				}

				//Sendֻ�ǻ������ݣ�����ʵ�ʷ���	
				memcpy(buffer, &m_msg._type, sizeof(m_msg._type));
				memcpy(buffer + sizeof(m_msg._type), &m_msg._size, sizeof(m_msg._size));
				memcpy(buffer + sizeof(m_msg._type) + sizeof(m_msg._size),
					m_msg._data, m_msg._size);

				ret = send(m_socket, buffer, sizeof(m_msg._type) + sizeof(m_msg._size) + m_msg._size, 0);
				

				if (ret == SOCKET_ERROR)
				{
					goto end;
				}
				delete[] m_msg._data;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Server_imp::StartListen(UINT16 port)
	{
		m_port = port;
		Start();
	}

	String Server_imp::GetClientIP(UINT32 id)
	{
		auto iter = m_clients.find(id);
		if (iter == m_clients.end())
		{
			return L"no client!";
		}
		else
		{
			return iter->second->ip;
		}
	}

	unsigned Server_imp::GetClientNum()
	{
		return m_clients.size();
	}

	void Server_imp::GetAllClient(ClientUser* users)
	{
		UINT32 i = 0;
		auto iter = m_clients.begin();
		for(;iter != m_clients.end(); ++iter)
		{
			users[i].id = iter->first;
			users[i].ip = iter->second->ip;
			i++;
		}
	}

	void Server_imp::SetProc(void(*func)(UINT32 id, NetMsg msg))
	{
		m_proc_func = func;
	}

	void Server_imp::SetProcBegin(void(*func)(UINT32 id))
	{
		m_proc_func_begin = func;
	}

	void Server_imp::SetProcEnd(void(*func)(UINT32 id))
	{
		m_proc_func_end = func;
	}

	void Server_imp::Send(UINT32 id, NetMsg msg)
	{
		auto iter = m_clients.find(id);
		if (iter == m_clients.end())
		{
			debug_err(L"DND: Server SendĿ�겻���ڡ�");
			return;
		}
		else
		{

			if (iter->second->thread.m_sending == 1)
			{
				debug_err(L"DND: Server��ʱ���õ���Send��");
				return;
			}

			//���������m_msg
			NetMsg temp = msg;
			temp._data = new BYTE[msg._size];
			memcpy(temp._data, msg._data, msg._size);

			iter->second->thread.m_msg = temp;


			unsigned send = 1;
			InterlockedExchange(&iter->second->thread.m_sending, send);
		}
	}


	String Server_imp::GetClientInfo(UINT32 id)
	{
		return String::Format(300, L"[%s] - %05d: ",
			GetClientIP(id).GetWcs(), id);
	}



	void Server_imp::_run()
	{
		//�����׽���
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			debug_err(L"DND: Server�����׽���ʧ�ܡ�");
			return;
		}


		//�󶨷�������ַ
		SOCKADDR_IN server_ip;
		server_ip.sin_family = AF_INET;
		server_ip.sin_addr.s_addr = INADDR_ANY;
		server_ip.sin_port = htons((short)m_port);

		int ret = bind(m_socket, (LPSOCKADDR)&server_ip, sizeof(server_ip));
		if (ret == SOCKET_ERROR)
		{
			debug_err(L"DND: Server�󶨵�ַʧ�ܡ�");
			return;
		}


		//���ܿͻ�������ѭ��
		listen(m_socket, 1);
		while (true)
		{
			//�ͻ�����Ϣ
			SOCKET client_new;
			sockaddr_in client_addr;
			int addr_len = sizeof(client_addr);

			client_new = accept(m_socket, (sockaddr FAR*)&client_addr, &addr_len);

			if (client_new == INVALID_SOCKET)
			{
				debug_err(L"DND: Server���ܿͻ�������ʧ�ܡ�");
				return;
			}

			

			ClientInfo* info = new ClientInfo;
			info->socket = client_new;
			/*char temp[32];
			inet_ntop(AF_INET, (void*)&client_addr, temp, 32);*/
			
			info->ip = String(inet_ntoa(client_addr.sin_addr));


			//�̼߳���
			unsigned lock = 1;
			InterlockedExchange(&m_lock, lock);

			_add_client(info);

			lock = 0;
			InterlockedExchange(&m_lock, lock);

		}


	}

	void Server_imp::_add_client(ClientInfo* info)
	{
		//��1��max��0Ϊ����ֵ
		for (unsigned i = 1; i < 200000; ++i)
		{
			auto iter = m_clients.find(i);
			if (iter == m_clients.end())
			{
				//������Ϣ��ǳ������
				m_clients[i] = info;
				//��������recv�߳�
				m_clients[i]->thread.Init(info->socket, i);

				debug_notice(GetClientInfo(i) + L"���յ�һ�����ӡ�");
				//���ûص�
				if(m_proc_func_begin)
					m_proc_func_begin(i);
				break;
			}
		}
	}

	Server_imp::~Server_imp()
	{
		closesocket(m_socket);
	}
}