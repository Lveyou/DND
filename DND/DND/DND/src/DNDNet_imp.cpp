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
		//Server只有一个
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
		//这里复制msg的buffer数据在堆上，等发送给服务器后释放。
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
		UINT32 state = 0;//开始
		InterlockedExchange(&m_state, state);

		char buffer[BUFFER_SIZE];
re2://断线重连
		if (m_end)
			return;
		//创建套接字
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			debug_err(L"DND:Client 创建套接字失败。");
			return;
		}

		//连接服务器
		SOCKADDR_IN server_ip;
		server_ip.sin_family = AF_INET;
		m_server_ip.GetMultiByteStr(buffer, BUFFER_SIZE);
		//inet_pton(AF_INET, buffer, (void*)&server_ip);
		server_ip.sin_addr.s_addr = inet_addr(buffer);
		server_ip.sin_port = htons((short)m_port);

re:	
		if (m_end)
			return;
		state = 1;//连接中
		InterlockedExchange(&m_state, state);
		int ret = connect(m_socket, (LPSOCKADDR)&server_ip, sizeof(server_ip));
		if (ret == SOCKET_ERROR)
		{
			state = -1;//失败
			InterlockedExchange(&m_state, state);
			debug_warn(L"DND: Clinet连接服务器失败。");
			Sleep(3000);//3秒后重连
			
			goto re;
			
		}

		debug_notice(L"DND: Clinet连接服务器成功。");

		//请求接受循环
		while (!m_end)
		{
			//如果没有消息发送 ，就sleep线程
			if (m_sends.size() == 0)
			{
				state = 2;//空闲
				InterlockedExchange(&m_state, state);
				Sleep(100);
				continue;
			}

			unsigned lock_send = 1;//禁止Send函数写入
			InterlockedExchange(&m_lock_send, lock_send);

			//从队列取出一个消息
			NetMsg msg = m_sends.front();
			
			lock_send = 0;//
			InterlockedExchange(&m_lock_send, lock_send);

			state = 3;//发送

			InterlockedExchange(&m_state, state);
			memcpy(buffer, &msg._type, sizeof(msg._type));
			memcpy(buffer + sizeof(msg._type), &msg._size, sizeof(msg._size));
			memcpy(buffer + sizeof(msg._type) + sizeof(msg._size),
				msg._data, msg._size);

			ret = send(m_socket, buffer, sizeof(msg._type) + sizeof(msg._size) + msg._size, 0);


			if (ret == SOCKET_ERROR)
			{
				debug_err(L"DND:Clinet 发送数据失败。");
				closesocket(m_socket);
				goto re2;
			}
			//成功发送之后，释放堆内存，再移出msg
			m_sends.pop_front();
			delete[] msg._data;

			state = 4;//接收
			InterlockedExchange(&m_state, state);
			ret = recv(m_socket, buffer, BUFFER_SIZE, 0);


			if (ret == SOCKET_ERROR)
			{
				debug_err(L"DND:Clinet 接收数据失败。");
				closesocket(m_socket);
				goto re2;
			}

			//根据收到的消息构造一个NetMsg，用户Unbuild后释放堆内存
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

			state = 2;//空闲，这时保证bufferrecv是正确的
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
				//调转到这里说明，对方已经断开了连接
				debug_warn(((Server_imp*)(Net::GetServer()))->GetClientInfo(m_id) + L"断开了连接");
				if (Server_imp::m_proc_func_end)
					Server_imp::m_proc_func_end(m_id);
				//结束线程，删除ClientInfo
				closesocket(m_socket);
				((Server_imp*)(Net::GetServer()))->m_clients.erase(m_id);
				//m_socket = INVALID_SOCKET;
				return;
			}
			else
			{
				//进入到这里，就说明客户端请求数据了
				unsigned lock_send = 0;
				InterlockedExchange(&m_sending, lock_send);

				//这里处理客户端发来的消息
				if (Server_imp::m_proc_func)
				{
					//这里自己负责new，消息处理函数处理后delete
					NetMsg msg2;
					memcpy(&msg2._type, buffer, sizeof(msg2._type));
					memcpy(&msg2._size, buffer + sizeof(msg2._type), sizeof(msg2._size));
					//发送的buffer过大，直接断开连接
					if (msg2._size > BUFFER_SIZE - sizeof(msg2._type) - sizeof(msg2._size))
					{
						goto end;
					}
					msg2._data = new BYTE[msg2._size];
					memcpy(msg2._data, buffer + sizeof(msg2._type) + sizeof(msg2._size), msg2._size);

					Server_imp::m_proc_func(m_id, msg2);

					delete[] msg2._data;
				}

				//这里会等待 m_msg被填充
				while (m_sending == 0)//没有数据需要发送
				{
					Sleep(100);
				}

				//Send只是缓存内容，这里实际发送	
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
			debug_err(L"DND: Server Send目标不存在。");
			return;
		}
		else
		{

			if (iter->second->thread.m_sending == 1)
			{
				debug_err(L"DND: Server此时不该调用Send。");
				return;
			}

			//这里深拷贝至m_msg
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
		//创建套接字
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			debug_err(L"DND: Server创建套接字失败。");
			return;
		}


		//绑定服务器地址
		SOCKADDR_IN server_ip;
		server_ip.sin_family = AF_INET;
		server_ip.sin_addr.s_addr = INADDR_ANY;
		server_ip.sin_port = htons((short)m_port);

		int ret = bind(m_socket, (LPSOCKADDR)&server_ip, sizeof(server_ip));
		if (ret == SOCKET_ERROR)
		{
			debug_err(L"DND: Server绑定地址失败。");
			return;
		}


		//接受客户端连接循环
		listen(m_socket, 1);
		while (true)
		{
			//客户端信息
			SOCKET client_new;
			sockaddr_in client_addr;
			int addr_len = sizeof(client_addr);

			client_new = accept(m_socket, (sockaddr FAR*)&client_addr, &addr_len);

			if (client_new == INVALID_SOCKET)
			{
				debug_err(L"DND: Server接受客户端连接失败。");
				return;
			}

			

			ClientInfo* info = new ClientInfo;
			info->socket = client_new;
			/*char temp[32];
			inet_ntop(AF_INET, (void*)&client_addr, temp, 32);*/
			
			info->ip = String(inet_ntoa(client_addr.sin_addr));


			//线程加锁
			unsigned lock = 1;
			InterlockedExchange(&m_lock, lock);

			_add_client(info);

			lock = 0;
			InterlockedExchange(&m_lock, lock);

		}


	}

	void Server_imp::_add_client(ClientInfo* info)
	{
		//从1到max，0为保留值
		for (unsigned i = 1; i < 200000; ++i)
		{
			auto iter = m_clients.find(i);
			if (iter == m_clients.end())
			{
				//加入信息（浅拷贝）
				m_clients[i] = info;
				//创建独立recv线程
				m_clients[i]->thread.Init(info->socket, i);

				debug_notice(GetClientInfo(i) + L"接收到一个连接。");
				//调用回调
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