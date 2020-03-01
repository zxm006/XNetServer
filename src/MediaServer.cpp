#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "../include/XNet/XNetCore.h"
#include "../include/XNet/XNetServer.h"
#include "TCPServer.h"
#include "aes.h"
#include <sys/time.h>
//#include "CheckAccredit.h"
static bool isencrypt =  false;
TCPServer tCServer;
unsigned int  GetCurrentTimeStamp(void);
#ifndef WIN32
#include <sys/types.h>
#include <sys/resource.h> 
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void DaemonInit(void)
{
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    signal(SIGHUP ,SIG_IGN);
    signal(SIGTERM,SIG_IGN);
    
	int pid = 0;
    
	if(pid==fork()>0)
	{
		exit(0);
	}
	else if(pid<0)
	{
		exit(1);	
	}
	setsid();
    
//    if(pid==fork()>0)
//    {
//        exit(0);
//    }
//    else if(pid<0)
//    {
//        exit(1);
//    }
	for(int fd=3,fdtablesize=getdtablesize();fd <fdtablesize;fd++)
	{
		close(fd);
	}

//	struct rlimit l = {0,0};
//	l.rlim_cur=65535;
//	l.rlim_max=65535;
//	setrlimit(RLIMIT_NOFILE,&l);
}
#else
#include<atlstr.h>
#endif

#ifndef WIN32
//  void checkExp(int exp)
//  {
//      CheckAccredit credite;
//      bool ret= false;
//      std::string info = "";
//      unsigned int   ulexpdate;
//      credite.check(ret,ulexpdate, info);
//      if (!ret) {
//       tCServer.Stop();
//       usleep(20000000/10);
//        printf("服务器证书失效:\"%s\"",info.c_str());
//      exit(0);
//
//      } else {
//             alarm(60*60*24);
//      }
//}

#else

void CALLBACK TimeProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
    CheckAccredit credite;
    
    bool ret= false;
    std::string info = "";
    unsigned int   ulexpdate;
    credite.check(ret,ulexpdate, info);
    
  if (!ret)
    {
        tCServer.Stop();
 
        Sleep(20000/10);
 
        printf("服务器证书失效:\"%s\"",info.c_str());
        exit(0);
    }
}


DWORD CALLBACK ThreadProc(PVOID pvoid)
{
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    SetTimer(NULL, 1000, 1000, TimeProc);
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(msg.message == WM_TIMER)
        {
            TranslateMessage(&msg);    // 翻译消息
            DispatchMessage(&msg);     // 分发消息
        }
    }
    KillTimer(NULL, 10);
    return 0;
}


#endif

unsigned int  GetCurrentTimeStamp(void)
{
#ifdef WIN32

struct timeval tv;  
        time_t clock;  
        struct tm tm;  
        SYSTEMTIME wtm;  
  
        GetLocalTime(&wtm);  
        tm.tm_year = wtm.wYear - 1970;  
        tm.tm_mon = wtm.wMonth - 1;  
        tm.tm_mday = wtm.wDay;  
        tm.tm_hour = wtm.wHour;  
        tm.tm_min = wtm.wMinute;  
        tm.tm_sec = wtm.wSecond;  
        tm.tm_isdst = -1;  
        clock = mktime(&tm);  
        tv.tv_sec = clock;  
        tv.tv_usec = wtm.wMilliseconds * 1000;  
        return ((unsigned int long)tv.tv_sec * 1000 + (unsigned int long)tv.tv_usec / 1000);
#else
    struct timeval now;
    gettimeofday(&now,NULL);
    return now.tv_sec*1000+now.tv_usec/1000;
#endif
}
void SetConsoleUtf8()
{
	#ifdef WIN32
 SetConsoleOutputCP (65001);  
    CONSOLE_FONT_INFOEX info = { 0 }; // 以下设置字体来支持中文显示。  
    info.cbSize = sizeof(info);  
    info.dwFontSize.Y = 14; // leave X as zero  
    info.FontWeight = FW_NORMAL;  
    wcscpy(info.FaceName,L"Lucida Console");  
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info); 
#endif
}

int main(int argc, char* argv[])
{
  
//  CheckAccredit credite;
//
//    bool ret= false;
//    std::string info = "";
//    unsigned int   ulexpdate;
//    credite.check(ret,ulexpdate, info);
//
//    if (!ret)
//    {
//
//    #ifndef WIN32
//         printf("%s",info.c_str());
//        usleep(10000);
//    #else
//        SetConsoleUtf8();
//        printf("%s",info.c_str());
//
//        Sleep(10000);
//      #endif
//        return  0;
//    }
//    else
    {
        
#ifndef WIN32
//        DaemonInit();
        /////定时检查
//      signal(SIGALRM, checkExp);
//        alarm(60*60*24);
#else
          SetConsoleUtf8();
        Sleep(1000);
        DWORD dwThreadId;
        // 创建线程
        HANDLE hThread = CreateThread(NULL, 0, ThreadProc, 0, 0, &dwThreadId);
#endif
   tCServer.Static(isencrypt);
//       printf("%s 启动成功sucess",info.c_str() );
        while (true)
        {
#ifndef WIN32
            usleep(20000000/10);
        #else
            Sleep(20000/10);
            #endif
        }
    }
  return 0;
}
