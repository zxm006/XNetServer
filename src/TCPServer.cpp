#include <stdio.h>
#include <assert.h>
#include "TCPServer.h"
#
#ifdef WIN32
#include<assert.h>
#else
#include <sys/time.h>
#endif
#define mtgMaxMembers 6

TCPServer::TCPServer(void)
{
    m_pCmdTCPServer = new CmdTCPServer;
}

TCPServer::~TCPServer(void)
{
    if (m_pCmdTCPServer)
        delete m_pCmdTCPServer;
}

void TCPServer::Static(bool isencrypt)
{
    if (m_pCmdTCPServer)
        m_pCmdTCPServer->startRecv(*this,isencrypt);
}

void TCPServer::Stop()
{
    if (m_pCmdTCPServer)
        m_pCmdTCPServer->stopRecv();
}

unsigned int   TCPServer::TimeGetTimestamp()
{
    
#ifdef WIN32
    return GetTickCount();//∫¡√Î
#else
    struct timeval now;
    gettimeofday(&now,NULL);
    return now.tv_sec*1000+now.tv_usec/1000;
#endif
}

bool TCPServer::CreatMtg(const std::string &mtgid,unsigned int& usrID,unsigned int& MtgType,std::string &info)
{
    MTGINFOLIST_MAP::iterator iter = m_Mtglist.find(mtgid);
    
    if (iter != m_Mtglist.end()) {
        info = "会议室已存在";
        return false;
    }
    
    CLIENTUSERINFOLIST_MAP::iterator useriter = m_UserInfoList.find(usrID);
    if (useriter!=m_UserInfoList.end()) {
        MTGINFOLIST* pMtginfolist = new MTGINFOLIST;
        MTG_TYPE type = (MTG_TYPE)MtgType;
        pMtginfolist->ulmaxMembers = mtgMaxMembers;
        
        if ( type == MTG_Common) {
            pMtginfolist->ulmaxMembers = mtgMaxMembers;
        } else {
            pMtginfolist->ulmaxMembers = 9999;
        }
        
        pMtginfolist->strMtgId.assign(mtgid);
        pMtginfolist ->strCreater.assign(useriter ->second->strUserName);
        pMtginfolist->type = (MTG_TYPE)MtgType;
        useriter->second->strMtgId.assign(mtgid);
        useriter->second->ulMtgTime = this->TimeGetTimestamp();
        CLIENTUSERINFOLIST *pUser  =   useriter->second;
        CLIENTUSERINFOLIST *pClientUserList = new CLIENTUSERINFOLIST;
        pClientUserList->ulUserID = pUser->ulUserID;
        pClientUserList->strUserName.assign(pUser->strUserName) ;
        pClientUserList->ulUserAudioID = pUser->ulUserAudioID;
        pClientUserList->strLocalIP.assign(pUser->strLocalIP);
        pClientUserList->strNATIP.assign(pUser->strNATIP);
        pClientUserList->strAddress.assign(pUser->strAddress);
        pClientUserList->strGroupId.assign(pUser->strGroupId);
        pClientUserList->strNickName.assign(pUser->strNickName);
        pClientUserList->strHeadUrl.assign(pUser->strHeadUrl);
        pClientUserList->ulLatitude = pUser->ulLatitude;
        pClientUserList->ulLongitude = pUser->ulLongitude;
        pClientUserList->ulLoginTime = pUser->ulLoginTime;
        pClientUserList->strMtgId.assign(mtgid);
        pUser->strMtgId.assign(mtgid);
        pMtginfolist->userList[usrID] = pClientUserList;
        info = "会议室创建成功.";
        
        m_Mtglist[mtgid] =pMtginfolist;
        
        //         printf("creat mtgid = %s",mtgid.c_str());
    }
    return true;
}

bool TCPServer:: DestroyMtg(const std::string &mtgid,unsigned int& usrID,std::string &info)
{
    MTGINFOLIST_MAP::iterator iter = m_Mtglist.find(mtgid);
    if (iter != m_Mtglist.end()) {
        CLIENTUSERINFOLIST_MAP::iterator useriter = iter->second->userList.find(usrID);
        
        if (useriter==iter->second->userList.end()) {
            info = "不是此会议室成员.";
            return false;
        }
        
        if(iter->second->strCreater !=useriter->second->strUserName) {
            info = "需要管理权限.";
            return false;
        }
        CLIENTUSERINFOLIST_MAP::iterator miter =  m_UserInfoList.begin();
        miter->second->strMtgId = "";
        
        CLIENTUSERINFOLIST_MAP::iterator it = iter->second->userList.begin();
        while (it != iter->second->userList.end()) {
            it->second->strMtgId = "";
            std::string strLogin = "MTGCMD";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "MTGCMD";
            rPacket.SetCMD(strCMD);
            rPacket.SetAttribUL("MTGCMD", MTG_DESTROY);
            rPacket.SetAttrib("USERNAME", useriter->second->strUserName);
            rPacket.SetAttribUL("USERID", usrID);
            if (m_pCmdTCPServer)
                m_pCmdTCPServer->sendData(it->second->ulUserID, rPacket);
            CLIENTUSERINFOLIST *pUser  =   it->second;
            if (pUser) {
                delete pUser;
                pUser =NULL;
            }
            iter->second->userList.erase(it++);
        }
        MTGINFOLIST* pMtgInfoList = iter->second;
        delete pMtgInfoList;
        m_Mtglist.erase(iter);
        info = "会议室解散成功.";
        return true;
    } else {
        info = "会议室不存在.";
        return false;
    }
}

bool TCPServer::JoinMtg(const std::string &mtgid,unsigned int& ulClientID,unsigned int& MtgType,std::string &info)
{
    //根据会议id找到会议
    MTGINFOLIST_MAP::iterator mtgIter = m_Mtglist.find(mtgid);
    
    if (mtgIter != m_Mtglist.end()) {
        //找到当前用户的信息
        CLIENTUSERINFOLIST_MAP::iterator userIter = m_UserInfoList.find(ulClientID);
        if(mtgIter->second->userList.size()<mtgIter->second->ulmaxMembers&&userIter!=m_UserInfoList.end()) {
            std::string strLogin = "MTGCMDREQ";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "MTGCMDREQ";
            rPacket.SetCMD(strCMD);
            rPacket.SetAttribBL("RESULT", true);
            std::string lstItems;
            for (CLIENTUSERINFOLIST_MAP::iterator mtgUserIter = mtgIter->second->userList.begin(); mtgUserIter != mtgIter->second->userList.end(); mtgUserIter++) {
                if (ulClientID == mtgUserIter->first) {
                    printf("JoinMtg=== >userIter->first == mtgUserIter->first");
                    info="已在会议室中.";
                    return false;
                }
                std::string strLogin = "MTGCMD";
                KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
                std::string strCMD = "MTGCMD";
                rPacket.SetCMD(strCMD);
                rPacket.SetAttribUL("MTGCMD", MTG_JOIN);
                rPacket.SetAttribUL("USERID", ulClientID);
                userIter->second->ulMtgTime = this->TimeGetTimestamp();
                rPacket.SetAttribUL("MTGTIME",  userIter->second->ulMtgTime);
                rPacket.SetAttrib("USERNAME", userIter->second->strUserName);
                
                if (m_pCmdTCPServer)
                    m_pCmdTCPServer->sendData(mtgUserIter->second->ulUserID, rPacket);
                
                KCmdItem item;
                item.SetAttrib("USERNAME", mtgUserIter->second->strUserName);
                item.SetAttribUL("USERID", mtgUserIter->second->ulUserID);
                item.SetAttribUL("USERAUDIOID", mtgUserIter->second->ulUserAudioID);
                item.SetAttribUN("USERVIDEOSEE", mtgUserIter->second->uiVideoCanSee);
                item.SetAttrib("LOCALIP", mtgUserIter->second->strLocalIP);
                item.SetAttrib("NATIP", mtgUserIter->second->strNATIP);
                item.SetAttrib("ADDRESS", mtgUserIter->second->strAddress);
                item.SetAttrib("NICKNAME", mtgUserIter->second->strNickName);
                item.SetAttrib("HEADURL", mtgUserIter->second->strHeadUrl);
                item.SetAttribDBL("LATITUDE", mtgUserIter->second->ulLatitude);
                item.SetAttribDBL("LONGITUDE", mtgUserIter->second->ulLongitude);
                item.SetAttribUL("MTGTIME",  mtgUserIter->second->ulMtgTime);
                lstItems += item.GetString() + "\r\n";
            }
            rPacket.SetItem(lstItems);
            rPacket.SetAttribUL("MTGCMDREQ", MTG_JOIN);
            userIter->second->strMtgId.assign(mtgid);
            
            CLIENTUSERINFOLIST *pUser  =   userIter->second;
            CLIENTUSERINFOLIST *pClientUserList = new CLIENTUSERINFOLIST;
            pClientUserList->ulUserID = pUser->ulUserID;
            pClientUserList->strUserName.assign(pUser->strUserName) ;
            pClientUserList->ulUserAudioID = pUser->ulUserAudioID;
            pClientUserList->strLocalIP.assign(pUser->strLocalIP);
            pClientUserList->strNATIP.assign(pUser->strNATIP);
            pClientUserList->strAddress.assign(pUser->strAddress);
            pClientUserList->strGroupId.assign(pUser->strGroupId);
            pClientUserList->strNickName.assign(pUser->strNickName);
            pClientUserList->strHeadUrl.assign(pUser->strHeadUrl);
            pClientUserList->ulLatitude = pUser->ulLatitude;
            pClientUserList->ulLongitude = pUser->ulLongitude;
            pClientUserList->ulLoginTime = pUser->ulLoginTime;
            pClientUserList->strMtgId.assign(mtgid);
            pUser->strMtgId.assign(mtgid);
            mtgIter->second->userList[ulClientID] = pClientUserList;
            if (m_pCmdTCPServer)
                m_pCmdTCPServer->sendData(ulClientID, rPacket);
            info="加入会议室成功.";
            return true;
        } else {
            info="会议室人数达到上限.";
        }
    } else {
        std::string strinfo ="";
        bool ret =  CreatMtg(mtgid, ulClientID,MtgType,strinfo);
        RegMtg(ulClientID,MTG_JOIN,ret,strinfo);
        return ret;
    }
    RegMtg(ulClientID,MTG_JOIN,false,info);
    return false;
}

bool TCPServer::ExitMtg(const std::string &mtgid,unsigned int& ulClientID,std::string &info)
{
    MTGINFOLIST_MAP::iterator Mtgiter = m_Mtglist.find(mtgid);
    
    if (Mtgiter != m_Mtglist.end()) {
        CLIENTUSERINFOLIST_MAP::iterator useriter = Mtgiter->second->userList.find(ulClientID);
        if (useriter!=Mtgiter->second->userList.end()) {
            std::string username = useriter->second->strUserName;
            CLIENTUSERINFOLIST_MAP::iterator it = Mtgiter->second->userList.begin();
            while (it != Mtgiter->second->userList.end()) {
                std::string strLogin = "MTGCMD";
                KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
                std::string strCMD = "MTGCMD";
                rPacket.SetCMD(strCMD);
                rPacket.SetAttribUL("MTGCMD", MTG_EXIT);
                rPacket.SetAttribUL("USERID", ulClientID);
                rPacket.SetAttrib("USERNAME", username);
                if (m_pCmdTCPServer)
                    m_pCmdTCPServer->sendData(it->second->ulUserID, rPacket);
                it++;
            }
            CLIENTUSERINFOLIST *pUser  =   useriter->second;
            if (pUser) {
                delete pUser;
                pUser =NULL;
            }
            Mtgiter->second->userList.erase(useriter);
            if (Mtgiter->second->userList.size()<=0) {
                MTGINFOLIST* pMtgInfoList = Mtgiter->second;
                delete pMtgInfoList;
                pMtgInfoList = NULL;
                m_Mtglist.erase(Mtgiter);
            }
            
            info ="退出成功.";
            CLIENTUSERINFOLIST_MAP::iterator user  =  m_UserInfoList.find(ulClientID);
            user->second->strMtgId = "";
            user->second->ulMtgTime = 0;
            return  true;
        } else {
            info ="不在此会议室中.";
            return  false;
        }
    }
    else  {
        info ="会议室不存在.";
        return  false;
    }
}

void TCPServer::RegMtg(unsigned int& ulClientID,MTG_CMD mtgCmd,bool ret,std::string strinfo)
{
    std::string strLogin = "MTGCMDREQ";
    KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
    std::string strCMD = "MTGCMDREQ";
    rPacket.SetCMD(strCMD);
    rPacket.SetAttribBL("RESULT", ret);
    rPacket.SetAttrib("INFO", strinfo);
    rPacket.SetAttribUL("MTGCMDREQ", mtgCmd);
    m_pCmdTCPServer->sendData(ulClientID, rPacket);
    
}

void TCPServer::OnDispatchMtgCmd( MTG_CMD mtgCmd ,unsigned int& ulClientID,unsigned int& MtgType,const std::string &mtgid)
{
    switch (mtgCmd)  {
        case  MTG_CREAT: {
            std::string strinfo ="";
            bool ret =  CreatMtg(mtgid, ulClientID,MtgType,strinfo);
            RegMtg(ulClientID,MTG_CREAT,ret,strinfo);
        }
            break;
            
        case  MTG_DESTROY:  {
            std::string strinfo ="";
            bool ret = DestroyMtg(mtgid, ulClientID,strinfo);
            RegMtg(ulClientID,MTG_DESTROY,ret,strinfo);
        }
            break;
            
        case  MTG_JOIN:  {
            std::string strinfo ="";
            JoinMtg(mtgid, ulClientID,MtgType,strinfo);
        }
            break;
        case  MTG_EXIT:  {
            std::string strinfo ="";
            bool ret =ExitMtg(mtgid, ulClientID,strinfo);
            RegMtg(ulClientID,MTG_EXIT,ret,strinfo);
            
        }
            break;
        default:
            break;
    }
}

void TCPServer::onLoginServer(unsigned int& ulClientID, KCmdPacketEx& pPacket)
{
    std::string strUserName = pPacket.GetAttrib("USERNAME").AsString();
    unsigned int ulUserAudioID = pPacket.GetAttrib("USERAUDIOID").AsUnsignedLong();
    std::string strLocalIP = pPacket.GetAttrib("LOCALIP").AsString();
    std::string strNATIP = pPacket.GetAttrib("NATIP").AsString();
    std::string strAddress = pPacket.GetAttrib("ADDRESS").AsString();
    std::string strGroupId = pPacket.GetAttrib("GROUPID").AsString();
    std::string strheadUrl =  pPacket.GetAttrib("HEADURL").AsString();
    std::string strnickName =  pPacket.GetAttrib("NICKNAME").AsString();
    double ulLatitude = pPacket.GetAttrib("LATITUDE").AsDouble();
    double ulLongitude = pPacket.GetAttrib("LONGITUDE").AsDouble();
    double ulloginTime =  this->TimeGetTimestamp();
    std::string strLogin = "LoginServered";
    KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
    std::string strCMD = "LOGINSERVERED";
    rPacket.SetCMD(strCMD);
    std::string strname = "USERID";
    rPacket.SetAttribUL(strname, ulClientID);
    std::string strLOGINTIME = "LOGINTIME";
    rPacket.SetAttribUL(strLOGINTIME, ulloginTime );
    
    CLIENTUSERINFOLIST_MAP::iterator it = m_UserInfoList.begin();
    while (it != m_UserInfoList.end())  {
        if (it->second->strAddress == strAddress&&it->second->strUserName == strUserName)  {
            unsigned int ulRemoteClientID = it->first;
            onReLoginServer(ulRemoteClientID);
            break;
        }
        it++;
    }
    //printf("Send Data to Client \n");
    rPacket.SetAttribUL("ERRORTYPE", 0);
    if (m_pCmdTCPServer)
        m_pCmdTCPServer->sendData(ulClientID, rPacket);
    
    SPEAKERLIST_MAP::iterator itspeaker = m_speakerList.find(strAddress);
    
    if (itspeaker != m_speakerList.end())  {
        std::string strLogin = "SENDVOICE";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        std::string strCMD = "SENDVOICE";
        rPacket.SetCMD(strCMD);
        std::string strname = "USERNAME";
        rPacket.SetAttrib(strname, itspeaker->second->strUserName);
        std::string strUserId = "USERID";
        rPacket.SetAttribUL(strUserId, itspeaker->second->ulUserID);
        std::string strUserAudioId = "USERAUDIOID";
        rPacket.SetAttribUN(strUserAudioId, itspeaker->second->ulUserAudioID);
        std::string stAddress = "ADDRESS";
        rPacket.SetAttrib(stAddress, itspeaker->second->strAddress);
        std::string stGroup = "GROUPID";
        rPacket.SetAttrib(stGroup, itspeaker->second->strGroupId);
        rPacket.SetAttribBL("ISINROOM", true);
        std::string strDATA = "DATA";
        rPacket.SetAttrib(strDATA, "AUDIO_SEND_DISABLE");
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulClientID, rPacket);
    }
    
    itspeaker = m_speakerList.find(strGroupId);
    
    if (itspeaker != m_speakerList.end())  {
        std::string strLogin = "SENDVOICE";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        std::string strCMD = "SENDVOICE";
        rPacket.SetCMD(strCMD);
        std::string strname = "USERNAME";
        rPacket.SetAttrib(strname, itspeaker->second->strUserName);
        std::string strUserId = "USERID";
        rPacket.SetAttribUL(strUserId, itspeaker->second->ulUserID);
        std::string strUserAudioId = "USERAUDIOID";
        rPacket.SetAttribUN(strUserAudioId, itspeaker->second->ulUserAudioID);
        std::string stAddress = "ADDRESS";
        rPacket.SetAttrib(stAddress, itspeaker->second->strAddress);
        std::string stGroup = "GROUPID";
        rPacket.SetAttrib(stGroup, itspeaker->second->strGroupId);
        rPacket.SetAttribBL("ISINROOM", false);
        
        std::string strDATA = "DATA";
        rPacket.SetAttrib(strDATA, "AUDIO_SEND_DISABLE");
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulClientID, rPacket);
        
    }
    
    //Õ®±®∆‰À˚”√ªß
    for (CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.begin(); iter != m_UserInfoList.end(); iter++) {
        std::string strLogin = "RemoteLoginServered";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        std::string strCMD = "REMOTEUSERLOGIN";
        rPacket.SetCMD(strCMD);
        std::string strname = "USERNAME";
        rPacket.SetAttrib(strname, strUserName);
        std::string strUserId = "USERID";
        std::string LocalIP = "LOCALIP";
        rPacket.SetAttrib(LocalIP, strLocalIP);
        std::string NATIP = "NATIP";
        rPacket.SetAttrib(NATIP, strNATIP);
        rPacket.SetAttribUL(strUserId, ulClientID);
        std::string strUserAudioId = "USERAUDIOID";
        rPacket.SetAttribUL(strUserAudioId, ulUserAudioID);
        std::string strLOGINTIME = "LOGINTIME";
        rPacket.SetAttribUL(strLOGINTIME, ulloginTime );
        std::string strUserVideoSee = "USERVIDEOSEE";
        rPacket.SetAttribUN(strUserVideoSee, 0);
        std::string strADDRESS = "ADDRESS";
        rPacket.SetAttrib(strADDRESS, strAddress);
        std::string strGROUPID = "GROUPID";
        rPacket.SetAttrib(strGROUPID, strGroupId);
        std::string headUrl = "HEADURL";
        rPacket.SetAttrib(headUrl, strheadUrl);
        std::string nickName = "NICKNAME";
        rPacket.SetAttrib(nickName, strnickName);
        std::string latitude = "LATITUDE";
        rPacket.SetAttribDBL(latitude, ulLatitude);
        std::string longitude = "LONGITUDE";
        rPacket.SetAttribDBL(longitude, ulLongitude);
        
        unsigned int ulRemoteClientID = iter->first;
        if (m_pCmdTCPServer&&iter->second->strAddress== strAddress)
            m_pCmdTCPServer->sendData(ulRemoteClientID, rPacket);
    }
    
    CLIENTUSERINFOLIST *pClientUserList = new CLIENTUSERINFOLIST;
    assert(pClientUserList);
    pClientUserList->ulUserID = ulClientID;
    pClientUserList->strUserName.assign(strUserName) ;
    pClientUserList->ulUserAudioID = ulUserAudioID;
    pClientUserList->strLocalIP.assign(strLocalIP);
    pClientUserList->strNATIP.assign(strNATIP);
    pClientUserList->strAddress.assign(strAddress);
    pClientUserList->strGroupId.assign(strGroupId);
    pClientUserList->strNickName.assign(strnickName);
    pClientUserList->strHeadUrl.assign(strheadUrl);
    pClientUserList->ulLatitude = ulLatitude;
    pClientUserList->ulLongitude = ulLongitude;
    pClientUserList->ulLoginTime = ulloginTime;
    
    pClientUserList->strMtgId.assign("");
    m_UserInfoList[ulClientID] = pClientUserList;
    
}

void TCPServer::onReLoginServer(  unsigned int& ulClientID )
{
    CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.find(ulClientID);
    if (iter != m_UserInfoList.end())  {
        if (iter->second->strMtgId != "")  {
            std::string strinfo ="";
            ExitMtg(iter->second->strMtgId, ulClientID,strinfo);
        }
        CLIENTUSERINFOLIST* pClientUserList = iter->second;
        assert(pClientUserList);
        std::string strUserName = pClientUserList->strUserName;
        std::string strAddress = pClientUserList->strAddress;
        std::string strGroupId = pClientUserList->strGroupId;
        delete pClientUserList;
        m_UserInfoList.erase(iter);
        SPEAKERLIST_MAP::iterator it = m_speakerList.find(strAddress);
        
        if (it != m_speakerList.end()) {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if (pVoiceUserList->ulUserID == ulClientID) {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        
        it = m_speakerList.find(strGroupId);
        if (it != m_speakerList.end()) {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if (pVoiceUserList->ulUserID == ulClientID) {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        
        //Õ®±®∆‰À˚”√ªß
        
        for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
            std::string strLogin = "RemoteQuitServered";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "REMOTEUSERQUIT";
            rPacket.SetCMD(strCMD);
            std::string strname = "USERNAME";
            rPacket.SetAttrib(strname, strUserName);
            std::string strUserId = "USERID";
            rPacket.SetAttribUL(strUserId, ulClientID);
            
            
            unsigned int ulRemoteClientID = nextiter->first;
            if (m_pCmdTCPServer&&strAddress== nextiter->second->strAddress&&ulRemoteClientID!=ulClientID)
                m_pCmdTCPServer->sendData(ulRemoteClientID, rPacket);
        }
        //        printf("%s  退出 \n",strUserName.c_str());
        
        std::string strLogin = "ReLogin";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        std::string strCMD = "RELOGIN";
        rPacket.SetCMD(strCMD);
        std::string strname = "USERNAME";
        rPacket.SetAttrib(strname, strUserName);
        std::string strUserId = "USERID";
        rPacket.SetAttribUL(strUserId, ulClientID);
        
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulClientID, rPacket);
    }
}

void TCPServer::onQuitServer(  unsigned int& ulClientID )
{
    CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.find(ulClientID);
    if (iter != m_UserInfoList.end()) {
        if (iter->second->strMtgId != "") {
            std::string strinfo ="";
            ExitMtg(iter->second->strMtgId, ulClientID,strinfo);
        }
        
        CLIENTUSERINFOLIST* pClientUserList = iter->second;
        assert(pClientUserList);
        std::string strUserName = pClientUserList->strUserName;
        std::string strAddress = pClientUserList->strAddress;
        std::string strGroupId = pClientUserList->strGroupId;
        
        if (pClientUserList) {
            delete pClientUserList;
            pClientUserList =NULL;
        }
        
        m_UserInfoList.erase(iter);
        SPEAKERLIST_MAP::iterator it = m_speakerList.find(strAddress);
        
        if (it != m_speakerList.end()) {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if (pVoiceUserList->ulUserID == ulClientID) {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        
        it = m_speakerList.find(strGroupId);
        if (it != m_speakerList.end()) {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if (pVoiceUserList->ulUserID == ulClientID) {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        
        //Õ®±®∆‰À˚”√ªß
        for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
            std::string strLogin = "RemoteQuitServered";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "REMOTEUSERQUIT";
            rPacket.SetCMD(strCMD);
            std::string strname = "USERNAME";
            rPacket.SetAttrib(strname, strUserName);
            std::string strUserId = "USERID";
            rPacket.SetAttribUL(strUserId, ulClientID);
            
            unsigned int ulRemoteClientID = nextiter->first;
            if (m_pCmdTCPServer&&strAddress== nextiter->second->strAddress&&ulRemoteClientID!=ulClientID)
                m_pCmdTCPServer->sendData(ulRemoteClientID, rPacket);
        }
        //    printf("%s  退出 \n",strUserName.c_str());
        
        std::string strLogin = "QuitServered";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        std::string strCMD = "USERQUIT";
        rPacket.SetCMD(strCMD);
        std::string strname = "USERNAME";
        rPacket.SetAttrib(strname, strUserName);
        std::string strUserId = "USERID";
        rPacket.SetAttribUL(strUserId, ulClientID);
        ;
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulClientID, rPacket);
    }
    
}

void TCPServer::OnsendVoice(unsigned int& ulClientID, KCmdPacketEx& pPacket)
{
    bool isinRoom = pPacket.GetAttrib("ISINROOM").AsBoolean();
    std::string strDATA = pPacket.GetAttrib("DATA").AsString();
    if (strDATA.compare("AUDIO_SEND_DISABLE") != 0)
    {
        std::string strAddress = pPacket.GetAttrib("ADDRESS").AsString();
        std::string strGroup = pPacket.GetAttrib("GROUPID").AsString();
        std::string strSpeaker;
        std::string strUserName = pPacket.GetAttrib("USERNAME").AsString();
        if (isinRoom) {
            strSpeaker = strAddress;
        } else {
            strSpeaker = strGroup;
        }
        SPEAKERLIST_MAP::iterator it = m_speakerList.find(strSpeaker);
        
        if (it != m_speakerList.end()) {
            if (strUserName != it->second->strUserName) {
                return;
            }
            CLIENTUSERINFOLIST* pClientUserList = it->second;
            assert(pClientUserList);
            delete pClientUserList;
            m_speakerList.erase(it);
            //Õ®±®∆‰À˚”√ªß
            for (CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.begin(); iter != m_UserInfoList.end(); iter++) {
                unsigned int ulRemoteClientID = iter->first;
                if(isinRoom) {
                    //                            printf("∑øº‰ƒ⁄Õ£÷π”Ô“Ù%s\n\r", strUserName.c_str());
                    
                    if (m_pCmdTCPServer&&ulRemoteClientID != ulClientID&&strAddress.compare(iter->second->strAddress)==0)
                        m_pCmdTCPServer->sendData(ulRemoteClientID, pPacket);
                }
                else {
                    //                            printf("◊Èƒ⁄Õ£÷π”Ô“Ù%s\n\r", strUserName.c_str());
                    if (m_pCmdTCPServer&&ulRemoteClientID != ulClientID&&strAddress.compare(iter->second->strAddress) == 0&& strGroup.compare(iter->second->strGroupId) == 0)
                        m_pCmdTCPServer->sendData(ulRemoteClientID, pPacket);
                }
                
            }
        }
        
        
        return;
    }
    else
    {
        std::string strUserName = pPacket.GetAttrib("USERNAME").AsString();
        unsigned int ulUserAudioID = pPacket.GetAttrib("USERAUDIOID").AsUnsignedLong();
        std::string strLocalIP = pPacket.GetAttrib("LOCALIP").AsString();
        std::string strNATIP = pPacket.GetAttrib("NATIP").AsString();
        std::string strAddress = pPacket.GetAttrib("ADDRESS").AsString();
        std::string strGroupId = pPacket.GetAttrib("GROUPID").AsString();
        std::string strSpeaker;
        
        if (isinRoom) {
            strSpeaker = strAddress;
        }else {
            strSpeaker = strGroupId;
        }
        SPEAKERLIST_MAP::iterator it = m_speakerList.find(strSpeaker);
        
        if (it != m_speakerList.end()) {
            std::string strLogin = "SENDVOICE";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "SENDVOICE";
            rPacket.SetCMD(strCMD);
            std::string strname = "USERNAME";
            rPacket.SetAttrib(strname, it->second->strUserName);
            std::string strUserId = "USERID";
            rPacket.SetAttribUL(strUserId, it->second->ulUserID);
            std::string strUserAudioId = "USERAUDIOID";
            rPacket.SetAttribUN(strUserAudioId, it->second->ulUserAudioID);
            std::string stAddress = "ADDRESS";
            rPacket.SetAttrib(stAddress, it->second->strAddress);
            std::string stGroup = "GROUPID";
            rPacket.SetAttrib(stGroup, it->second->strGroupId);
            rPacket.SetAttribBL("ISINROOM", isinRoom);
            std::string strDATA = "DATA";
            rPacket.SetAttrib(strDATA, "AUDIO_SEND_DISABLE");
            if (m_pCmdTCPServer)
                m_pCmdTCPServer->sendData(ulClientID, rPacket);
        }
        else {
            std::string strLogin = "SENDVOICE";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "SENDVOICE";
            rPacket.SetCMD(strCMD);
            std::string strname = "USERNAME";
            rPacket.SetAttrib(strname, strUserName);
            std::string strUserId = "USERID";
            rPacket.SetAttribUL(strUserId, ulClientID);
            std::string strUserAudioId = "USERAUDIOID";
            std::string stAddress = "ADDRESS";
            rPacket.SetAttrib(stAddress, strAddress);
            std::string stGroup = "GROUPID";
            rPacket.SetAttrib(stGroup, strGroupId);
            rPacket.SetAttribBL("ISINROOM", isinRoom);
            
            if (isinRoom) {
                strSpeaker = strAddress;
            } else {
                strSpeaker = strGroupId;
            }
            
            std::string strDATA = "DATA";
            rPacket.SetAttrib(strDATA, "AUDIO_SEND_ENABLE");
            if (m_pCmdTCPServer)
                m_pCmdTCPServer->sendData(ulClientID, rPacket);
            CLIENTUSERINFOLIST* pClientUserList = new CLIENTUSERINFOLIST;
            assert(pClientUserList);
            pClientUserList->ulUserID = ulClientID;
            pClientUserList->strUserName = strUserName;
            pClientUserList->ulUserAudioID = ulUserAudioID;
            pClientUserList->strLocalIP = strLocalIP;
            pClientUserList->strNATIP = strNATIP;
            pClientUserList->strAddress = strAddress;
            pClientUserList->strGroupId = strGroupId;
            
            if (isinRoom) {
                //                          printf("∑øº‰ƒ⁄ø™ º”Ô“Ù%s\n\r", strUserName.c_str());
                m_speakerList[strAddress] = pClientUserList;
            } else {
                //                          printf("◊Èƒ⁄ø™ º”Ô“Ù%s\n\r", strUserName.c_str());
                m_speakerList[strGroupId] = pClientUserList;
            }
            
            //Õ®±®∆‰À˚”√ªßshi
            for (CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.begin(); iter != m_UserInfoList.end(); iter++) {
                unsigned int ulRemoteClientID = iter->first;
                
                if (isinRoom) {
                    if (m_pCmdTCPServer&&ulRemoteClientID != ulClientID&&strAddress.compare(iter->second->strAddress) == 0)
                        m_pCmdTCPServer->sendData(ulRemoteClientID, pPacket);
                } else {
                    if (m_pCmdTCPServer&&ulRemoteClientID != ulClientID&&strAddress.compare(iter->second->strAddress) == 0 && strGroupId.compare(iter->second->strGroupId) == 0)
                        m_pCmdTCPServer->sendData(ulRemoteClientID, pPacket);
                }
            }
        }
    }
}
void TCPServer::OnDispatchCmd(unsigned int& ulClientID, KCmdPacketEx& pPacket)
{
    std::string strCMD = pPacket.GetCMD();
    std::string strData = pPacket.GetString();
    
    if (strCMD == "LOGINSERVER") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        onLoginServer(ulClientID, pPacket);
    } else if (strCMD == "GETUSERLIST") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        std::string strLogin = "UpdateUserList";
        std::string strCMD = "UPDATEUSERLIST";
        KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
        rPacket.SetCMD(strCMD);
        std::string lstItems;
        
        std::string	strADDRESS = pPacket.GetAttrib("ADDRESS").AsString();
        
        for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
            std::string strAddress = nextiter->second->strAddress;
            
            if (strADDRESS.compare(strAddress)!=0||nextiter->first==ulClientID) {
                continue;
            }
            std::string strUserName = nextiter->second->strUserName;
            KCmdItem item;
            item.SetAttrib("USERNAME", strUserName);
            item.SetAttribUL("USERID", nextiter->second->ulUserID);
            item.SetAttribUL("USERAUDIOID", nextiter->second->ulUserAudioID);
            item.SetAttribUN("USERVIDEOSEE", nextiter->second->uiVideoCanSee);
            item.SetAttrib("LOCALIP", nextiter->second->strLocalIP);
            item.SetAttrib("NATIP", nextiter->second->strNATIP);
            item.SetAttrib("ADDRESS", nextiter->second->strAddress);
            item.SetAttrib("NICKNAME", nextiter->second->strNickName);
            item.SetAttrib("HEADURL", nextiter->second->strHeadUrl);
            item.SetAttribDBL("LATITUDE", nextiter->second->ulLatitude);
            item.SetAttribDBL("LONGITUDE", nextiter->second->ulLongitude);
            item.SetAttribUL("LOGINTIME", nextiter->second->ulLoginTime );
            
            lstItems += item.GetString() + "\r\n";
        }
        rPacket.SetItem(lstItems);
        //printf("Send Data to Client \n");
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulClientID, rPacket);
    } else if (strCMD == "VIDEOCALL") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        unsigned int ulPeerUserID = pPacket.GetAttrib("PEERUSERID").AsUnsignedLong();
        for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
            if (nextiter->first == ulPeerUserID) {
                //            printf("Send VIDEOCALL to Client %lu \n",ulPeerUserID);
                unsigned int ulUserId = nextiter->first;
                if (m_pCmdTCPServer) {
                    //                    printf("Send VIDEOCALL to Client %lu \n", ulPeerUserID);
                    m_pCmdTCPServer->sendData(ulUserId, pPacket);
                }
                
                break;
            }
        }
        
    } else if (strCMD == "INVITEMTGUSER") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        unsigned int ulUserId = pPacket.GetAttrib("PEERUSERID").AsUnsignedLong();
        if (m_pCmdTCPServer)
            m_pCmdTCPServer->sendData(ulUserId, pPacket);
    }
    
    else if (strCMD == "SIGNALINGTRANSFER") {
        bool isbroadcast = pPacket.GetAttrib("BROADCAST").AsBoolean();
        unsigned int ulUserID = pPacket.GetAttrib("USERID").AsUnsignedLong();
        std::string ulUserName = pPacket.GetAttrib("USERNAME").AsString();
        if (isbroadcast) {
          std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
            std::string senddata = pPacket.GetAttrib("DATA").AsString();
            //Õ®±®∆‰À˚”√ªß
            for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
                if (nextiter->first != ulUserID) {
                    //printf("Send Data to Client \n");
                    unsigned int ulUserId = nextiter->first;
                    if (m_pCmdTCPServer)
                        m_pCmdTCPServer->sendData(ulUserId, pPacket);
                }
            }
        } else {
          std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
            unsigned int ulPeerUserID = pPacket.GetAttrib("PEERUSERID").AsUnsignedLong();
            std::string senddata = pPacket.GetAttrib("DATA").AsString();
            //            printf("senddata = %s\n\r",senddata.c_str());
            for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
                if (nextiter->first == ulPeerUserID) {
                    //printf("Send Data to Client \n");
                    unsigned int ulUserId = nextiter->first;
                    if (m_pCmdTCPServer)
                        m_pCmdTCPServer->sendData(ulUserId, pPacket);
                    break;
                }
            }
        }
    } else if (strCMD == "SENDVOICE") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        OnsendVoice(ulClientID, pPacket);
    }
    else if (strCMD == "USERMEDIASTATUS")
    {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.find(ulClientID);
        if (iter != m_UserInfoList.end()) {
            CLIENTUSERINFOLIST* pClientUserList = iter->second;
            assert(pClientUserList);
            std::string strUserName = pClientUserList->strUserName;
            std::string strAddress = pClientUserList->strAddress;
            std::string strGroupId = pClientUserList->strGroupId;
            
            bool isvideo = pPacket.GetAttrib("ISVIDEO").AsBoolean();
            bool isopen = pPacket.GetAttrib("ISOPEN").AsBoolean();
            
            for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++) {
                std::string strLogin = "userMediaStatus";
                KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
                std::string strCMD = "USERMEDIASTATUS";
                rPacket.SetCMD(strCMD);
                std::string strname = "USERNAME";
                rPacket.SetAttrib(strname, strUserName);
                std::string strUserId = "USERID";
                rPacket.SetAttribUL(strUserId, ulClientID);
                rPacket.SetAttribBL("ISVIDEO", isvideo);
                rPacket.SetAttribBL("ISOPEN", isopen);
                //printf("Send Data to Client \n");
                unsigned int ulRemoteClientID = nextiter->first;
                if (m_pCmdTCPServer&&strAddress== nextiter->second->strAddress&&ulClientID!=ulRemoteClientID)
                    m_pCmdTCPServer->sendData(ulRemoteClientID, rPacket);
            }
        }
    } else if (strCMD == "MTGCMD") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        MTG_CMD meetingCommond   = (MTG_CMD)pPacket.GetAttrib("MTGCMD").AsUnsignedLong();
        std::string strUserName = pPacket.GetAttrib("USERNAME").AsString();
        std::string strMtgid = pPacket.GetAttrib("MTGID").AsString();
        unsigned int ulMtgType = pPacket.GetAttrib("MTGTYPE").AsUnsignedInt();
        
        if (strMtgid == "") {
            std::string strinfo = "会议室id不能为空";
            RegMtg(ulClientID,meetingCommond,false,strinfo);
            return;
        }
        OnDispatchMtgCmd(meetingCommond, ulClientID,ulMtgType,strMtgid);
        
    } else if (strCMD == "QUITSERVER") {
      std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
        //        printf("QUITSERVER\n\r");
        onQuitServer(ulClientID);
    }
}

void TCPServer::ClientConnected(unsigned int ulClientID, unsigned int ulClientRemoteIP)
{
    // printf("OnXNetServerClientConnected ulClientID=%lu ulClientRemoteIP=%lu\n\r", ulClientID, ulClientRemoteIP);
}

void TCPServer::ClientDisconnected(unsigned int ulClientID)
{
  std::lock_guard<std::recursive_mutex> lock_guard(m_mutex);
    
    CLIENTUSERINFOLIST_MAP::iterator iter = m_UserInfoList.find(ulClientID);
    if (iter != m_UserInfoList.end())
    {
        std::string strUserName = iter->second->strUserName;
        
        if (iter->second->strMtgId != "")
        {
            std::string strinfo ="";
            ExitMtg(iter->second->strMtgId, ulClientID,strinfo);
            
        }
        
        //         printf("OnXNetServerClientDisconnected ulClientID=%lu strUserNam = %s \n\r", ulClientID, strUserName.c_str());
        
        CLIENTUSERINFOLIST* pClientUserList = iter->second;
        assert(pClientUserList);
        std::string strAddress = pClientUserList->strAddress;
        std::string strGroupid = pClientUserList->strGroupId;
        if (pClientUserList) {
            delete pClientUserList;
            pClientUserList =NULL;
        }
        
        m_UserInfoList.erase(iter);
        
        SPEAKERLIST_MAP::iterator it = m_speakerList.find(strAddress);
        
        if (it != m_speakerList.end())
        {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if(pVoiceUserList->ulUserID== ulClientID)
            {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        
        it = m_speakerList.find(strGroupid);
        if (it != m_speakerList.end())
        {
            CLIENTUSERINFOLIST* pVoiceUserList = it->second;
            if (pVoiceUserList->ulUserID == ulClientID)
            {
                delete pVoiceUserList;
                m_speakerList.erase(it);
            }
        }
        //Õ®±®∆‰À˚”√ªß
        for (CLIENTUSERINFOLIST_MAP::iterator nextiter = m_UserInfoList.begin(); nextiter != m_UserInfoList.end(); nextiter++)
        {
            std::string strLogin = "RemoteQuitServered";
            KCmdPacketEx rPacket(strLogin.c_str(), (int)strLogin.length() + 1);
            std::string strCMD = "REMOTEUSERQUIT";
            rPacket.SetCMD(strCMD);
            std::string strname = "USERNAME";
            rPacket.SetAttrib(strname, strUserName);
            std::string strUserId = "USERID";
            rPacket.SetAttribUL(strUserId, ulClientID);
            //printf("Send Data to Client \n");
            unsigned int ulRemoteClientID = nextiter->first;
            if (m_pCmdTCPServer&&strAddress == nextiter->second->strAddress)
                m_pCmdTCPServer->sendData(ulRemoteClientID, rPacket);
        }
    }
    
}

int TCPServer::SendDataToUser(unsigned int uPeerUserID, const char* pData, std::string strUserName, unsigned int nLen)
{
    if (!pData || nLen == 0)
        return -1;
    //
    std::string strData(pData, nLen);
    
    std::string strinfo = "Signalingtransfer";
    KCmdPacketEx rPacket(strinfo.c_str(), (int)strinfo.length() + 1);
    std::string strCMD = "SIGNALINGTRANSFER";
    rPacket.SetCMD(strCMD);
    rPacket.SetAttribBL("BROADCAST", false);
    rPacket.SetAttribUL("PEERUSERID", uPeerUserID);
    rPacket.SetAttrib("DATA", strData);
    rPacket.SetAttrib("USERNAME", strUserName);
    if (m_pCmdTCPServer)
        m_pCmdTCPServer->sendData(uPeerUserID, rPacket);
    return 0;
}
