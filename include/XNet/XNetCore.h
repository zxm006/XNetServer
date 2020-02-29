#ifndef __XNetCORE_H__
#define __XNetCORE_H__

#include "XNetDefine.h"

class XNetAPI XNetCore
{
public:
	XNetCore(void);
	virtual~XNetCore(void);
public:
	//启动内核
	//成功返回0，失败返回错误码
	static int Start(unsigned short usPort=0,bool bHTTPPort=false,bool bAnyUDPPort=false,bool bHasMCU=true);
	//停止内核
	static void Stop(void);

	static unsigned short GetUDPPort(void);
	static const char*GetLocalIP(void);
	static SOCKET GetSocketHandle(unsigned int ulIP);

	static void SetUpdateFilePath(const char*cszFilePath);
	static bool HasMCU(void);
	static bool IsMCUClient(void);
	static bool IsMCUServer(void);
};

//返回数据
typedef void (*OnUDPDispatchCmd)(unsigned int& ulClientID,const char* czsbuf, unsigned int len,unsigned int ulIP);

#endif
