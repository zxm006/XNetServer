//XNetServer.h
#ifndef __XNetSERVER_H__
#define __XNetSERVER_H__

#include "XNetSession.h"

//网络Server
class XNetServerCallback
{
public:
	XNetServerCallback(void){};
	virtual~XNetServerCallback(void){};
public:
	virtual void OnXNetServerClientConnected(unsigned int ulClientID,unsigned int ulClientRemoteIP)=0;
	virtual void OnXNetServerClientDisconnected(unsigned int ulClientID)=0;
	virtual void OnXNetServerReceivedFromClient(unsigned int ulClientID,const char*pData,int nLen)=0;

};

class XNetAPI XNetServer
{
public:
	XNetServer(void){};
	virtual ~XNetServer(void){};
public:
	//启动本地服务
	virtual int Start(void)=0;
	
	//停止本地服务
	virtual void Stop(void)=0;

	//获得客户SESSION数目
	virtual unsigned int GetClientCount(void)=0;

	//发送数据给指定的网络客户端
	virtual int SendDataToClient(unsigned int nClientID,const char*pData,int nLen)=0;

	//强制断开客户端
	virtual void DisconnectClient(unsigned int nClientID)=0;

	static XNetServer*Create(XNetServerCallback&rCallback,unsigned short usServerType);
};

#endif
