//
//  CheckAccredit.cpp
//  ImServer-netec
//
//  Created by zhangxinming on 2017/11/10.
//  Copyright © 2017年 zhangxinming. All rights reserved.
//
//
#include "pem.h"

#include "CheckAccredit.h"
#include "Utils.h"
#include <sstream>
#include <iostream>

#ifdef    HttpTool
#include "curl/curl.h"
#include "json.h"
#endif

#ifndef WIN32
#include<sys/time.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include<atlstr.h>
#endif

#include <string.h>
#include <stdio.h>

//      Json::Value root;
//    Json::Value arrayObj;
//    Json::Value item;
//
//
//    item["uid"]=Json::Value("chechenluoyang@163.com");
//    item["fileName"]=Json::Value("梅西.txt");
//    item["time"]=Json::Value("2017.07.28 10:55:22");
//    item["type"]=Json::Value("Libcurl HTTP POST JSON");
//    item["authList"]=Json::Value("test");
//    arrayObj.append(item);

//    root  = arrayObj;
//    std::string jsonout = root.toStyledString();

//    std::string sstr="服务器名称-版本号-申请日期-授权到期时间-ip地址(数组)-其他用户信息";
//    std::string pri="./pri.key";
//  std::string enstr =  my_encrypt((char*)sstr.c_str(),(char*)pri.c_str());
//    printf("enstr %s\n\r",enstr.c_str());
////
//  std::string enstr="WHDf3IX56Fp328u7+xW9mWjQsDYrwt87l+EDXRyuyjaGLD6MVdGwEvO+tievL3NY9+Z8EL6NbsgKBelqGNl8JB5CmJ/Y1Rm7zGx1krptLyPL8pENvAEM0HWnThZfFnrMj9fmfiTwaWJC14/SSbiTmVn0r2dkmEeguWm0FNGm/dg=";
//   char* enstr= readCertificate();
//
//  std::string destr = my_decrypt((char*)enstr,NULL);

//    printf("destr %s\n\r",destr.c_str());

CheckAccredit::CheckAccredit() 
{
 
}

//---------------------------------------------------------------------------------------
CheckAccredit::~CheckAccredit()
{
  
}
 void CheckAccredit::getIPAddress(std::string &address)
{
 #ifndef WIN32
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if( temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                std::string ifa_name = "";
                ifa_name.assign(temp_addr->ifa_name);
                
                if ( ifa_name.compare("en0") == 0) {
                    // Get NSString from C String
                    address.assign(inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr));
        
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
#else
    
    char strHost[128] = { 0 };
    
    // get host name, if fail, SetLastError is called
    if (SOCKET_ERROR != gethostname(strHost, sizeof(strHost)))
    {
        struct hostent* hp;
        hp = gethostbyname(strHost);
        if (hp != NULL && hp->h_addr_list[0] != NULL)
        {
            // IPv4: Address is four bytes (32-bit)
            if ( hp->h_length < 4)
                return ;
            
            // Convert address to . format
            strHost[0] = 0;
            
            // IPv4: Create Address string
            sprintf(strHost, "%u.%u.%u.%u",
                    (UINT)(((PBYTE) hp->h_addr_list[0])[0]),
                    (UINT)(((PBYTE) hp->h_addr_list[0])[1]),
                    (UINT)(((PBYTE) hp->h_addr_list[0])[2]),
                    (UINT)(((PBYTE) hp->h_addr_list[0])[3]));
            
            address = strHost;
            return ;
        }
    }
    else
        SetLastError(ERROR_INVALID_PARAMETER);
#endif
    
  
}


time_t CheckAccredit::convert(int year,int month,int day)
{
    tm info={0};
    info.tm_year=year-1900;
    info.tm_mon=month-1;
    info.tm_mday=day;
    return mktime(&info);
}


unsigned long CheckAccredit::GetTimeStamp(void)
{
#ifdef WIN32
    return GetTickCount();//∫¡√Î
#else
    struct timeval now;
    gettimeofday(&now,NULL);
    return now.tv_sec*1000+now.tv_usec/1000;
#endif
}


int  CheckAccredit::get_days(const char* from, const char* to)
{
    int year,month,day;
    sscanf(from,"%d-%d-%d",&year,&month,&day);
    int fromSecond=(int)convert(year,month,day);
    sscanf(to,"%d-%d-%d",&year,&month,&day);
    int toSecond=(int)convert(year,month,day);
    return (toSecond-fromSecond)/24/3600;
}
#ifdef WIN32
void ConvertGBKToUtf8(CString& strGBK) {
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0);
    unsigned short * wszUtf8 = new unsigned short[len+1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1,(LPWSTR) wszUtf8, len);
    
    len = WideCharToMultiByte(CP_UTF8, 0,(LPWSTR) wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8=new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);
    
    strGBK = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
}

void ConvertUtf8ToGBK(CString& strUtf8) {
    int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0);
    unsigned short * wszGBK = new unsigned short[len+1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1,(LPWSTR) wszGBK, len);
    
    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK=new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
    
    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}
#endif


 void CheckAccredit::check(bool &isAvailabe,   unsigned long & ulexpdate,std::string & info)
{
   std::string localip = "";
    getIPAddress(localip);
    char* enstr= readCertificate();
    if(!enstr)
    {
        isAvailabe = false;
        info = "没有授权证书 error.";
        return;
    }
    bool isipavailable = false;
    std::string certificate = my_decrypt((char*)enstr,NULL);
    delete []enstr;
    enstr = NULL;
    if(certificate == "")
    {
        isAvailabe = false;
        info = "没有授权信息 error.";
        return;
    }
    char  pDecode[128]  ;
    memset(pDecode, 0,certificate.size());
    memcpy(pDecode, certificate.c_str(), certificate.size());
    char * serverVer = NULL;
    char   split[] = ";";
   serverVer = strtok(pDecode,split);

//    #ifdef WIN32
//    CString  strver = serverVer;
//    ConvertUtf8ToGBK(strver);
//  //  printf("server %s",strver.GetString() );
//	    info = strver.GetString();
//#else
    info = serverVer;

//#endif
    char * expDate = strtok(NULL, split);
    char * iplist = strtok(NULL, split);
  
   ulexpdate = strtoul(expDate,NULL,10);
 
          time_t   now;
        struct   tm     *timenow;
        time(&now);
        timenow   =   localtime(&now);
        char datebuffer[32];
        sprintf( datebuffer,"%d-%d-%d", timenow->tm_year+1900, timenow->tm_mon+1, timenow->tm_mday);
           int days=get_days(datebuffer,expDate);
            if (days<=0) {
                isAvailabe = false;
                info = "授权期已过 error";
                return;
            }

  
 
    char *ip = strtok(iplist, ",");
    while(ip)
    {
       // printf("%s ", ip);
        if (localip.compare(ip) == 0 )
        {
            isipavailable = true;
            break;
        }
        ip = strtok(NULL, ",");
    }
 
//    Json::Reader reader;
//    Json::Value value;
 
//
//    if (reader.parse(jsonvalse, value))
//    {
//        const std::string expDate = value["expDate"].asString();
//       int days=get_days(datebuffer,expDate.c_str());
//        if (days<=0) {
//            isAvailabe = false;
//            info = "证书已过有效期";
//            return;
//        }
//
//        const Json::Value arrayObj = value["ipList"];
//     for (unsigned int i = 0; i < arrayObj.size(); i++)
//        {
//            if (!arrayObj[i].isMember("ip"))
//                continue;
//            std::string ip = arrayObj[i]["ip"].asString();
//          if(ip.compare(localip)==0)
//            {
//                isipavailable = true;
//                break;
//            }
//       }
//    }

     if (!isipavailable)
     {
         isAvailabe = false;
         info = "证书不适用于此ip error";
         return;
     }
	 else
	 {
        isAvailabe = true;
	 }
}

std::string CheckAccredit::my_encrypt(char *inputchar, char *prikey_path)
{

    FILE *fp = NULL;

    if ((fp = fopen(prikey_path, "r")) == NULL) {
        return NULL;
    }

//PEM_read_RSAPublicKEY()只能读取RSA PUBLIC KEY形式的公钥文件；而PEM_read_RSA_PUBKEY()只能读取PUBLIC KEY格式的公钥文件
    /* 读取公钥PEM，PUBKEY格式PEM使用PEM_read_RSA_PUBKEY函数 */
    RSA* pRSAPriKey = RSA_new();
    if(PEM_read_RSAPrivateKey(fp, &pRSAPriKey, NULL, NULL) == NULL)
    {
        return  NULL;
    }

    std::string outbuff ;
    Base64Encode(inputchar, outbuff);

    unsigned long size =outbuff.size();

unsigned char * pEcode = new unsigned char [size];
        memset(pEcode,0,size);
    memcpy(pEcode, outbuff.c_str(), size);

    int nLen = RSA_size(pRSAPriKey);
          nLen-=11;
    char* pDecode = new char[nLen];


    memset(pDecode,0,nLen);
    if (RSA_check_key(pRSAPriKey) < 1) {
        printf("generate_key: key generation failed\n");
        exit(-1);
    }
    if (RSA_private_encrypt(nLen, (unsigned char *)pEcode, (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING) < 0) {
        return NULL;
    }

    RSA_free(pRSAPriKey);
    fclose(fp);
    std::string renbuff = pDecode ;
    std::string oubuff;
    Base64Encode(renbuff, oubuff);
    return  oubuff ;

}
//std::string strPemKey ="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCG9YGC1pERIucMSCIppAIVC1WXNvLQ1XuyXqsEO2ncIDw/i5mZHx4QZXeh7XKLyFWxtzW4q4ZMxG5kulxY9zlveITNsHfpH8WUm/Ef/uU1P+nyAJd/YRF31TX+cdk3lsWpm2Y5gGZEoNlACy/bSX4Jq/sQzH0P72buRMlNmuxDEQIDAQAB";
std::string strPemKey ="MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCubOmUDrCUbptD8+BN2UxcCtuJHjx04AFnQSEnzBN+eSBdx8I8NjA4+3TBGhvpxHwzh5dIOa3zHRrwkVuNSKjd2fAU4fuW07DkJlg8htqWCwwpw2RUs0uKYf5/LnHzByies+LOWnguTtydBOq3P8tPTWCS70Q5DddYRPzkr7VgzwIDAQAB";

std::string CheckAccredit::my_decrypt(char *inputchar, char *pubkey_path)
{
    
  RSA *pRSAPublicKey = formatPublicRSAKey(strPemKey.c_str());
    int nLen = RSA_size(pRSAPublicKey);
    
    char* pDecode = new char[nLen+1];
    memset(pDecode, 0, nLen+1);
    
    std::string rinbuff  = inputchar;
    std::string routbuff ;
    Base64Decode(rinbuff, routbuff);
    unsigned long len = routbuff.size();
    if (len>128) {
        
        return  "";
    }
   if (RSA_public_decrypt((int)len, (unsigned char *)routbuff.c_str(), (unsigned char*)pDecode, pRSAPublicKey, RSA_PKCS1_PADDING) < 0)
    {
        return  "";
    }
    
    std::string inbuff  = pDecode;
    std::string outbuff ;
    Base64Decode(inbuff, outbuff);
    RSA_free(pRSAPublicKey);
    delete []pDecode;
    pDecode = NULL;
    return  outbuff ;
}

char* CheckAccredit::readCertificate()
{
    FILE * pFile;
    long lSize;
    char * buffer;
    size_t result;
    
    #ifdef WIN32
    int max =128;
    CString    sPath;
    GetModuleFileName(NULL,sPath.GetBufferSetLength(max+1),max);
    sPath.ReleaseBuffer    ();
    int    nPos;
    nPos=sPath.ReverseFind('\\');
    sPath=sPath.Left(nPos);
    
    CString szCommand;
    szCommand.Format(_T("%s//%s"), sPath, "certificate" );
   pFile = fopen (szCommand, "rb" );
#else
   pFile = fopen ("./certificate", "rb" );
#endif
//
    
    
    if (pFile==NULL)
    {
        return NULL;
    }
    
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    buffer = (  char*) malloc (sizeof(  char)*lSize+1);
     memset(buffer, 0,  sizeof(  char)*lSize+1);
    if (buffer == NULL)
    {
     
         return NULL;
    }
    
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize)
    {
 
        return NULL;
    }
    fclose (pFile);
    return buffer;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
//回调函数
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    //    string data((const char*) ptr, (size_t) size * nmemb);
    //
    //    *((stringstream*) stream) << data << endl;
    //
    //    return size * nmemb;
    return 0;
}



void CheckAccredit::http_post()
{
#ifdef    HttpTool
 

    CURL *curl;
    CURLcode res;
    char tmp_str[256] = { 0 };
    
    std::stringstream  out ;
    
    //HTTP报文头
    struct curl_slist* headers = NULL;
    
   char *url = "http://if.qdocument.net:705/bic/operationNote/upload";
//
//    curl = curl_easy_init();
//
   if(curl)
    {
//        //构建json
//        Json::Value item;
//        item["uid"]=Json::Value("chechenluoyang@163.com");
//        item["fileName"]=Json::Value("梅西&内马尔&苏亚雷斯.txt");
//        item["time"]=Json::Value("2017.07.31 9:55:22");
//        item["type"]=Json::Value("Libcurl HTTP POST Json串");
//        item["authList"]=Json::Value("weidong0925@126.com");
       std::string jsonout = "";//item.toStyledString();
       
        
        
        //设置url
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        //设置http发送的内容类型为JSON
        //构建HTTP报文头
        sprintf(tmp_str, "Content-Length: %s", jsonout.c_str());
        headers=curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
        //headers=curl_slist_append(headers, tmp_str);//在请求头中设置长度,请求会失败,还没找到原因
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        //curl_easy_setopt(curl,  CURLOPT_CUSTOMREQUEST, "POST");//自定义请求方式
        curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
        
        // 设置要POST的JSON数据
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonout.size());//设置上传json串长度,这个设置可以忽略
        
        // 设置接收数据的处理函数和存放变量
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);//设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据
        res = curl_easy_perform(curl);//执行
        curl_slist_free_all(headers); /* free the list again */
        std::string str_json = out.str();//返回请求值
        printf("%s",str_json.c_str());
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    #endif
}




RSA * CheckAccredit::formatPublicRSAKey(std::string str_key)
{
    int n_len = str_key.size();      //str_key为base64编码的公钥字符串
    for(int i = 64; i < n_len; i+=64)
    {
        if(str_key[i] != '\n')
        {
            str_key.insert(i, "\n");
        }
        i++;
    }
    
    str_key.insert(0, "-----BEGIN PUBLIC KEY-----\n");
    str_key.append("\n-----END PUBLIC KEY-----\n");
    
    BIO *bio = NULL;
    RSA *rsa = NULL;
    char *chPublicKey = const_cast<char *>(str_key.c_str());
    if ((bio = BIO_new_mem_buf(chPublicKey, -1)) == NULL)       //从字符串读取RSA公钥
    {
        //        cout<<"BIO_new_mem_buf failed!"<<endl;
    }
    else
    {
        rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
        if (!rsa)
        {
            BIO_free_all(bio);
        }
    }
    
    return rsa;
}

