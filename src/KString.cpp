#include <string.h>
#include "KString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//=======================================================================================
// KString类实现
//=======================================================================================
//---------------------------------------------------------------------------------------
std::string UPPERCASE(const std::string& source);

//---------------------------------------------------------------------------------------
std::string LOWERCASE(const std::string& source);
//---------------------------------------------------------------------------------------
std::string TRIMLEFT(const std::string& source, const char* chars2remove);

//---------------------------------------------------------------------------------------
std::string TRIMRIGHT(const std::string& source, const char* chars2remove);

//---------------------------------------------------------------------------------------
std::string TRIMSPACE(const std::string& source);

//---------------------------------------------------------------------------------------
std::string LONG2STR(long value);

//---------------------------------------------------------------------------------------
std::string ULONG2STR(unsigned int value);


//---------------------------------------------------------------------------------------
std::string INT2STR(int value);

//---------------------------------------------------------------------------------------
std::string UINT2STR(unsigned int value);

//---------------------------------------------------------------------------------------
std::string SHORT2STR(short value);

//---------------------------------------------------------------------------------------
std::string USHORT2STR(unsigned short value);

//---------------------------------------------------------------------------------------
std::string BYTE2STR(unsigned char value);

//---------------------------------------------------------------------------------------
std::string BOOL2STR(bool value);
//---------------------------------------------------------------------------------------
std::string DOUBLE2STR(double value);

//---------------------------------------------------------------------------------------
long STR2LONG(const std::string& value);
//---------------------------------------------------------------------------------------
unsigned int STR2ULONG(const std::string& value);

//---------------------------------------------------------------------------------------
int STR2INT(const std::string& value);

//---------------------------------------------------------------------------------------
unsigned int STR2UINT(const std::string& value);

//---------------------------------------------------------------------------------------
short STR2SHORT(const std::string& value);

//---------------------------------------------------------------------------------------
unsigned short STR2USHORT(const std::string& value);

//---------------------------------------------------------------------------------------
unsigned char STR2BYTE(const std::string& value);

//---------------------------------------------------------------------------------------
bool STR2BOOL(const std::string& value);

//---------------------------------------------------------------------------------------
double STR2DOUBLE(const std::string& value);

//---------------------------------------------------------------------------------------
unsigned short BOOL2USHORT(bool value);

//---------------------------------------------------------------------------------------
bool USHORT2BOOL(unsigned short value);

//---------------------------------------------------------------------------------------
std::string ULONG2HEX(unsigned int value);

//---------------------------------------------------------------------------------------
unsigned int HEX2ULONG(const std::string& value);

//---------------------------------------------------------------------------------------
std::string NONULLSTR(const char* value);
//---------------------------------------------------------------------------------------
KString::KString(void)
{
}

//---------------------------------------------------------------------------------------
KString::KString(const std::string& str)
{
	m_string = str;
}

//---------------------------------------------------------------------------------------
KString::~KString(void)
{
}

//---------------------------------------------------------------------------------------
KString& KString::operator=(KString& xstr)
{
	m_string = xstr.m_string;
	return *this;
}

//---------------------------------------------------------------------------------------
KString& KString::operator=(const std::string& str)
{
	m_string = str;
	return *this;
}

//---------------------------------------------------------------------------------------
std::string KString::AsString(void)
{
	return m_string;
}

//---------------------------------------------------------------------------------------

double KString::AsDouble(void)
{
    return STR2DOUBLE(m_string);
    
}


//---------------------------------------------------------------------------------------
unsigned int KString::AsUnsignedLong(void)
{
	return STR2ULONG(m_string);
}

//---------------------------------------------------------------------------------------
unsigned int KString::AsUnsignedInt(void)
{
	return STR2UINT(m_string);
}

//---------------------------------------------------------------------------------------
unsigned short KString::AsUnsignedShort(void)
{
	return STR2USHORT(m_string);
}

//---------------------------------------------------------------------------------------
unsigned char KString::AsUnsignedChar(void)
{
	return STR2BYTE(m_string);
}

//---------------------------------------------------------------------------------------
bool KString::AsBoolean(void)
{
	return STR2BOOL(m_string);
}

//=======================================================================================
// KBitString类实现
//=======================================================================================

//---------------------------------------------------------------------------------------
KBitString::KBitString(void)
{
    memset(m_szPrivilegeString,0,sizeof(m_szPrivilegeString));
    memset(m_szPrivilegeString,'N',sizeof(m_szPrivilegeString)-1);
}

//---------------------------------------------------------------------------------------
KBitString::KBitString(const char* cszPrivilegeString)
{
    memset(m_szPrivilegeString,0,sizeof(m_szPrivilegeString));
    memcpy(m_szPrivilegeString,cszPrivilegeString,strlen(cszPrivilegeString));
}

//---------------------------------------------------------------------------------------
KBitString::~KBitString(void)
{
}

//---------------------------------------------------------------------------------------
void KBitString::SetPrivilege(int nBit,bool bEnable)
{
    if(nBit>255)
    {
        return;
    }
    char nBitValue=bEnable?'Y':'N';
    m_szPrivilegeString[nBit]=nBitValue;
}

//---------------------------------------------------------------------------------------
bool KBitString::GetPrivilege(int nBit)
{
    if(nBit<0 || nBit>255)
    {
        return false;
    }
    char nBitValue = m_szPrivilegeString[nBit];
    if(nBitValue=='Y')
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------
const char* KBitString::GetString(void)
{
    return m_szPrivilegeString;
}
