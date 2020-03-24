#include <stdio.h>
#include <stdlib.h>
#include <map>

#ifdef WIN32
#include <assert.h>
#endif

#include "../include/XNet/XNetCore.h"
#include "../include/XNet/XNetServer.h"
#include "CmdTCPServer.h"
#include <string.h>
//#include "aes.h"
//
//#define CBC 1
//#define ECB 1
//
//
//uint8_t kcrypt[] = {0x1b, 0x6e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa9, 0x1b, 0xf7, 0x85, 0x88, 0x09, 0xcf, 0x4f, 0x11};

class CServer: public XNetServerCallback
{
public:
    CServer(CmdTCPServerCallback& pCallback);
    virtual ~CServer(void);
public:
    int Open(bool isEncrypt);
    void Close(void);
    void ClearClients(void);
    bool SendData(unsigned int& ulClientID,KCmdPacketEx& pPacket);
    
public:
        void OnXNetServerClientConnected(unsigned int ulClientID,unsigned int ulClientRemoteIP);
      void OnXNetServerClientDisconnected(unsigned int ulClientID);
      void OnXNetServerReceivedFromClient(unsigned int ulClientID,const char*pData,int nLen);

protected:
    typedef std::map<unsigned int,unsigned int> MapClientID;
    
    XNetServer*    m_pServer;
    MapClientID        m_MapClientID;
    
public:
    void SetFunPointer(CmdTCPServerCallback* pCallback);
private:
    CmdTCPServerCallback& m_pCallback;
    bool m_isEncrypt;
};

CServer::CServer(CmdTCPServerCallback& pCallback)
:m_pCallback(pCallback)
{
    m_pServer=NULL;
    m_isEncrypt = true;
}

CServer::~CServer(void)
{
    
}

int CServer::Open(bool isEncrypt)
{
    XNetCore::Start(5566,false,false,true);
    m_pServer=XNetServer::Create(*this,1);
    m_pServer->Start();
    m_isEncrypt=isEncrypt;
    return 0;
}

void CServer::Close(void)
{
    if (m_pServer) {
        m_pServer->Stop();
        delete m_pServer;
        m_pServer=NULL;
    }
}

 
void CServer::OnXNetServerClientConnected(unsigned int ulClientID,unsigned int ulClientRemoteIP)
{
    m_pCallback.ClientConnected(ulClientID, ulClientRemoteIP);
    m_MapClientID[ulClientID]=ulClientID;
//    printf("OnXNetServerClientConnected ulClientID=%u ulClientRemoteIP=%u\n",ulClientID,ulClientRemoteIP);
}

void CServer::OnXNetServerClientDisconnected(unsigned int ulClientID)
{
    m_MapClientID.erase(ulClientID);
    m_pCallback.ClientDisconnected(ulClientID);
//     printf("OnXNetServerClientDisconnected ulClientID=%u \n",ulClientID);
}

void CServer::OnXNetServerReceivedFromClient(unsigned int ulClientID,const char*pData, int nLen)
{
        KCmdPacketEx rPacket((const char *)pData,nLen);
        char cClientID[255];
        rPacket.SetDomain("");
        rPacket.SetSourceType(0);
        memset(cClientID,0,255);
    sprintf(cClientID,"%u",ulClientID);
        rPacket.SetSourceID(cClientID);
        m_pCallback.OnDispatchCmd(ulClientID,rPacket);
}

bool CServer::SendData(unsigned int& ulClientID,KCmdPacketEx& pPacket)
{
    if(m_MapClientID.find(ulClientID) == m_MapClientID.end() )
        return false;
 
        std::string data = pPacket.GetString();
        m_pServer->SendDataToClient(ulClientID,(const char*)data.c_str(),data.length()+1);
 
    return true;
}

void CServer::ClearClients(void)
{
    MapClientID::iterator iter = m_MapClientID.begin();
    while (iter!=m_MapClientID.end()) {
        if (m_pServer!=NULL) {
            m_pServer->DisconnectClient(iter->second);
        }
        ++iter;
    }
    m_MapClientID.clear();
}

bool CmdTCPServer::startRecv( CmdTCPServerCallback& pCallback,bool isEncrypt)
{
    m_pTCPServer = new CServer(pCallback);
    m_pTCPServer->Open(isEncrypt);
    return true;
}

bool CmdTCPServer::stopRecv()
{
    m_pTCPServer->Close();
    return true;
}
bool CmdTCPServer::sendData(unsigned int& ulClientID, KCmdPacketEx& pPacket)
{
    m_pTCPServer->SendData(ulClientID,pPacket);
    return true;
}

