//
//  CheckAccredit.hpp
//  ImServer-netec
//
//  Created by zhangxinming on 2017/11/10.
//  Copyright © 2017年 zhangxinming. All rights reserved.
//

#ifndef CheckAccredit_hpp
#define CheckAccredit_hpp

 #include "rsa.h"
#include <iostream>
#include <stdio.h>

class CheckAccredit
{
public:
    CheckAccredit();
    ~CheckAccredit();
    void check(bool &isAvailabe, unsigned long &ulexpdate,std::string & info);
protected:
    void  getIPAddress(std::string &address);
    std::string my_encrypt(char *inputchar, char *prikey_path);
    std::string my_decrypt(char *inputchar, char *pubkey_path);
    RSA *formatPublicRSAKey(std::string str_key);
    char* readCertificate();
    void  http_post();
    time_t convert(int year,int month,int day);
    int get_days(const char* from, const char* to);
    unsigned long  GetTimeStamp(void);
};

#endif /* CheckAccredit_hpp */
