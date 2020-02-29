#pragma once
#include "cmdpacket.h"

class CServer;

class CmdTCPServerCallback
{
public:
	CmdTCPServerCallback(void){};
	virtual~CmdTCPServerCallback(void){};
public:
	//返回客户端数据
	virtual void OnDispatchCmd(unsigned int & ulClientID,KCmdPacketEx& pPacket){};
	//返回客户端连接消息
	virtual void ClientConnected(unsigned int ulClientID,unsigned int ulClientRemoteIP){};
	//返回客户端断开消息
	virtual void ClientDisconnected(unsigned int ulClientID){};
};

class CmdTCPServer
{
public:
	bool startRecv(CmdTCPServerCallback& pCallback,bool isEncrypt);
	bool stopRecv();
	bool sendData(unsigned int& ulClientID,KCmdPacketEx& pPacket);
private:
	CServer* m_pTCPServer;
//     bool m_isEncrypt;
};


