#include "DNDNet_imp.h"
#include "DNDDebug.h"

namespace DND
{
	Client* Net::GetClient()
	{
		dnd_assert(_bClient, ERROR_00047);
		static Client_imp* c = new Client_imp;
		return c;
	}

	Server* Net::GetServer()
	{
		dnd_assert(!_bClient, ERROR_00048);
		static Server_imp* s = new Server_imp;
		return s;
	}

	int Net::_bClient = -1;

	void Client_imp::Connect(const String& ip, const int port)
	{
		m_server_ip = ip;
		m_port = port;
		m_lock_send = 0;
		m_lock_recv = 0;
		Start();
	}

	void Client_imp::Send(const NetMsg& msg)
	{
		m_sends.push_back(msg);
	}

	NetMsg Client_imp::Recv()
	{
		NetMsg ret = m_sends.front();
		m_sends.pop_front();
		return ret;
	}

	Client::State Client_imp::GetState()
	{
		return m_state;
	}

	void Client_imp::_run()
	{
		unsigned state = 0;//开始
		InterlockedExchange(&m_state, state);

		char buffer[BUFFER_SIZE];
		//创建套接字
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			Debug::Instance()->Write_Line(L"DND:Clinet 创建套接字失败！");
		}


		//连接服务器

		SOCKADDR_IN server_ip;
		server_ip.sin_family = AF_INET;
		m_server_ip.Get_Multi_Byte_Str(buffer, BUFFER_SIZE);
		inet_pton(AF_INET, buffer, (void*)&server_ip);
		server_ip.sin_port = htons((short)m_port);

	re:
		state = 1;//连接中
		InterlockedExchange(&m_state, state);
		int ret = connect(m_socket, (LPSOCKADDR)&server_ip, sizeof(server_ip));
		if (ret == SOCKET_ERROR)
		{
			state = -1;//失败
			InterlockedExchange(&m_state, state);
			Debug::Instance()->Write_Line(L"DND:Clinet 连接服务器失败！");
			Sleep(3000);//3秒后重连
			goto re;
		}

		Debug::Instance()->Write_Line(L"DND:Clinet 连接服务器成功！");

		//请求接受循环
		while (true)
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

			m_buffer_send = m_sends.front();
			m_sends.pop_front();

			lock_send = 0;//
			InterlockedExchange(&m_lock_send, lock_send);

			state = 3;//发送

			InterlockedExchange(&m_state, state);
			m_buffer_send.Get_Multi_Byte_Str(buffer, BUFFER_SIZE);

			//没有文件发送
			if (!m_files.empty())
			{
				char temp[] = "_file";
				FileBlock b = m_files.front();

				char name[MAX_PATH];
				char path[MAX_PATH];
				b.name.Get_Multi_Byte_Str(name, MAX_PATH);
				b.path.Get_Multi_Byte_Str(path, MAX_PATH);

				send(m_socket, temp, strlen(temp) + 1, 0);
				send(m_socket, name, strlen(name) + 1, 0);

				/*int length = 0;
				while ((length = send(m_socket, b.m_file_buffer, FILE_SIZE, 0)) > 0)
				{*/
				FILE * fp;
				fopen_s(&fp, path, "rb");  //windows下是"rb",表示打开一个只读的二进制文件  
				if (NULL == fp)
				{
					//printf("File: %s Not Found\n", file_name);
				}
				else
				{
					m_files.pop_front();
					memset(buffer, 0, BUFFER_SIZE);
					int length = 0;

					while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
					{
						if (send(m_socket, buffer, length, 0) < 0)
						{
							//printf("Send File: %s Failed\n", file_name);
							break;
						}
						memset(buffer, 0, BUFFER_SIZE);
					}

					fclose(fp);
					Debug::Instance()->Write_Line(L"DND:Clinet 文件发送成功！");

					Sleep(5000);//休息5s再发送

					ret = send(m_socket, temp, strlen(temp) + 1, 0);


				}

				/*	strcpy_s(temp, "_fend");

				send(m_socket, temp, strlen(temp) + 1, 0);
				Debug::Instance()->Write_Line(L"DND:Clinet 文件发送成功2！");*/
			}
			//Debug::Instance()->Write_Line(L"DND:Clinet 文件发送成功3！");
			ret = send(m_socket, buffer, strlen(buffer) + 1, 0);


			if (ret == SOCKET_ERROR)
			{
				Debug::Instance()->Write_Line(L"DND:Clinet 发送数据失败！");
				goto re;
			}
			////成功发送之后
			m_buffer_send = L"";

			state = 4;//接收
			InterlockedExchange(&m_state, state);
			ret = recv(m_socket, buffer, BUFFER_SIZE, 0);


			if (ret == SOCKET_ERROR)
			{
				Debug::Instance()->Write_Line(L"DND:Clinet 接收数据失败！");
				goto re;
			}

			m_buffer_recv = buffer;

			unsigned lock = 1;
			InterlockedExchange(&m_lock_recv, lock);

			m_recvs.push_back(m_buffer_recv);
			m_buffer_recv = L"";

			lock = 0;
			InterlockedExchange(&m_lock_recv, lock);



			state = 2;//空闲，这时保证bufferrecv是正确的
			InterlockedExchange(&m_state, state);

			/*unsigned sending = 0;
			InterlockedExchange(&m_sending, sending);*/
		}
	}

	Client_imp::~Client_imp()
	{
		closesocket(m_socket);
	}
}