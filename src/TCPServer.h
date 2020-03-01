#pragma once
#include "cmdtcpserver.h"
#include <mutex>

typedef enum {
    MTG_CREAT=0,		//正在连接
    MTG_JOIN,
    MTG_EXIT,
    MTG_DESTROY,
    MTG_INVITE,
    
} MTG_CMD;

typedef enum {
    MTG_Common=0,
    MTG_MultIntercom,
 } MTG_TYPE;


typedef struct _CLIENTUSERINFOLIST
{
	_CLIENTUSERINFOLIST():strUserName("")
		,ulUserID(0)
		,ulUserAudioID(0)
		,uiVideoCanSee(0)
		,strAddress("")
		,strGroupId("")
       ,strMtgId("")
       ,strHeadUrl ("")
       ,strNickName("")
       ,ulLatitude(0)
       ,ulLongitude(0)
       ,ulMtgTime(0)
       ,ulLoginTime()
    {}
	std::string strUserName;
    std::string strHeadUrl;
    std::string strNickName;
    double ulLatitude;
    double ulLongitude;
    
    unsigned int ulUserID;
	unsigned int ulUserAudioID;
    unsigned int ulMtgTime;
     unsigned int ulLoginTime;
	unsigned int  uiVideoCanSee;
    
	std::string strLocalIP;
	std::string strNATIP;
	std::string strAddress;
    
	std::string strGroupId;
    std::string strMtgId;
}CLIENTUSERINFOLIST;


typedef std::map<unsigned int, CLIENTUSERINFOLIST*> CLIENTUSERINFOLIST_MAP;

typedef std::map<std::string, CLIENTUSERINFOLIST*> SPEAKERLIST_MAP;


typedef struct _MTGINFOLIST
{
    _MTGINFOLIST():strMtgId(""),ulmaxMembers(6)
    {
        
    }
    std::string strMtgId;
    std::string strCreater;
    int ulmaxMembers;
    MTG_TYPE type;
    CLIENTUSERINFOLIST_MAP userList;
}MTGINFOLIST;


typedef std::map<std::string, MTGINFOLIST*> MTGINFOLIST_MAP;


class TCPServer : public CmdTCPServerCallback
{
public:
	TCPServer(void);
public:
	~TCPServer(void);
public:

	//返回客户端连接消息
	virtual void ClientConnected(unsigned int ulClientID,unsigned int ulClientRemoteIP);
	//返回客户端断开消息
	virtual void ClientDisconnected(unsigned int ulClientID);
public:
	void Static(bool isencrypt);
	void Stop();
	
private:
    //返回客户端数据
    virtual void OnDispatchCmd(unsigned int& ulClientID,KCmdPacketEx& pPacket);
    
	int SendDataToUser(unsigned int uPeerUserID ,const char* pData,std::string strUserName, unsigned int nLen);
    bool  CreatMtg(const std::string &mtgid,unsigned int& ulClientID,unsigned int& MtgType,std::string &info);
    bool  JoinMtg(const std::string &mtgid,unsigned int& ulClientID,unsigned int& MtgType,std::string &info);
    bool  ExitMtg(const std::string &mtgid,unsigned int& ulClientID,std::string &info);
    bool  DestroyMtg(const std::string &mtgid,unsigned int& ulClientID,std::string &info);
    void  RegMtg(unsigned int& ulClientID,MTG_CMD mtgCmd,bool ret,std::string strinfo);
    
    unsigned int   TimeGetTimestamp();
    
    void  onLoginServer(unsigned int& ulClientID, KCmdPacketEx& pPacket);
    void  onQuitServer(  unsigned int& ulClientID );
    void  OnsendVoice(unsigned int& ulClientID, KCmdPacketEx& pPacket);
    void  onReLoginServer(  unsigned int& ulClientID );
    
    void  OnDispatchMtgCmd( MTG_CMD mtgCmd ,unsigned int& usrID,unsigned int& MtgType,const std::string &mtgid);
	CmdTCPServer* m_pCmdTCPServer;
	CLIENTUSERINFOLIST_MAP m_UserInfoList;
    MTGINFOLIST_MAP m_Mtglist;
	SPEAKERLIST_MAP m_speakerList;
    std::recursive_mutex    m_mutex;
};
