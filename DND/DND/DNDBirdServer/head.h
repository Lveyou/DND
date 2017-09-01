#pragma once

#include <DND.h>
using namespace DND;
#include "NetMoulde.h"

class BirdServer : public Game
{
	virtual void _update() override;				//frame func
	virtual void _init() override;				//init
	virtual void _release() override;

	//��Ϣ�ַ���
	static void MsgDispatcher(UINT32 id, NetMsg msg);
	//cs_Info: ��ʾ�ͻ��˷������ַ���
	static void OnMsg_cs_Info(UINT32 id, cs_Info* msg);
	//cs_Beat: ��������
	static void OnMsg_cs_Beat(UINT32 id, cs_Beat* msg);
	static Server* server;
};

Server* BirdServer::server = NULL;

void BirdServer::_init()
{
	server = Net::GetServer();
	
	server->SetProc(MsgDispatcher);
	server->StartListen(4002);
	

}

void BirdServer::_update()
{
	Debug::GetDebugger<DebuggerConsole>()->Input();
}

void BirdServer::_release()
{
	
}


void BirdServer::MsgDispatcher(UINT32 id, NetMsg msg)
{
	DND_SERVER_MSG_HEAD()
	
	DND_SERVER_ON_MSG(cs_Info)
	DND_SERVER_ON_MSG(cs_Beat)

	debug_warn(server->GetClientInfo(id) + L"�յ��˴������Ϣ��");

}

void BirdServer::OnMsg_cs_Info(UINT32 id, cs_Info* msg)
{
	

	debug_msg(server->GetClientInfo(id) + msg->wcsInfo);

	sc_Ok msg_ok;
	NetMsg net_msg;
	net_msg.Build<sc_Ok>(&msg_ok);
	server->Send(id, net_msg);
}

void BirdServer::OnMsg_cs_Beat(UINT32 id, cs_Beat* msg)
{
	sc_Beat msg_beat;
	NetMsg net_msg;
	net_msg.Build<sc_Beat>(&msg_beat);

	server->Send(id, net_msg);
}
