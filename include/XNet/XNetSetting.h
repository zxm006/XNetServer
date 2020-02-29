//XNetSetting.h
#ifndef __XNetSETTING_H__
#define __XNetSETTING_H__

#include <XNet/XNetDefine.h>
//网络设置
class XNetAPI XNetSetting
{ 
public:
	XNetSetting(void);
	virtual~XNetSetting(void);
	
public:
	typedef enum{
		PT_NONE=0,		//无代理，直接连接
		PT_HTTP_PROXY,	//HTTP代理
		PT_SOCK5,		//SOCK5代理
		PT_HTTP_TUNNEL,	//HTTP隧道
		PT_HTTPS_TUNNEL	//HTTPS隧道
	}PROXY_TYPE;

	typedef enum{
		PT_TCP=0,		//TCP传输协议
		PT_UDP,			//UDP传输协议
		PT_RTP,			//RTP传输协议
	}PROTOCOL_TYPE;
	
public:
	//设置网络通讯协议类型
	static void SetSessionProtocolType(XNetSetting::PROTOCOL_TYPE type);
	static XNetSetting::PROTOCOL_TYPE GetSessionProtocolType(void);

	//设置音频传输协议类型
	static void SetAudioProtocolType(XNetSetting::PROTOCOL_TYPE type);
	static XNetSetting::PROTOCOL_TYPE GetAudioProtocolType(void);

	//设置视频传输协议类型
	static void SetVideoProtocolType(XNetSetting::PROTOCOL_TYPE type);
	static XNetSetting::PROTOCOL_TYPE GetVideoProtocolType(void);

	//设置数据传输协议类型
	static void SetDataProtocolType(XNetSetting::PROTOCOL_TYPE type);
	static XNetSetting::PROTOCOL_TYPE GetDataProtocolType(void);

	//设置是否只使用MCU
	static void SetMCUOnly(int nMCUOnly);
	static int GetMCUOnly(void);
	
	//设置代理类型
	static void SetProxyType(XNetSetting::PROXY_TYPE pt);
	static XNetSetting::PROXY_TYPE GetProxyType(void);
	
	//设置代理用户
	static void SetProxyUser(const char* cszUser);
	static const char*GetProxyUser(void);
	
	//设置代理密码
	static void SetProxyPassword(const char* cszPassword);
	static const char*GetProxyPassword(void);
	
	//设置代理服务器
	static void SetProxyHost(const char* cszHost);
	static const char*GetProxyHost(void);
	static void SetProxyPort(unsigned short usPort);
	static unsigned short GetProxyPort(void);
};

#endif
