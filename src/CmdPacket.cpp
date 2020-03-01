#include <stdio.h>
#include "CmdPacket.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------------------------
std::string UPPERCASE(const std::string& source)
{
	std::string destination=source;
	std::transform(destination.begin(), destination.end(), destination.begin(), ::toupper);
	return destination;
}

//---------------------------------------------------------------------------------------
std::string LOWERCASE(const std::string& source)
{
	std::string destination=source;
	std::transform(destination.begin(), destination.end(), destination.begin(), ::tolower);
	return destination;
}

//---------------------------------------------------------------------------------------
std::string TRIMLEFT(const std::string& source, const char* chars2remove)
{
	if(source.empty())
	{
		return "";
	}
	std::string destination=source;
	std::string::size_type pos = destination.find_first_not_of(chars2remove);
	if(pos != std::string::npos)
	{
		destination.erase(0,pos);
	}
	else
	{
		destination.erase(destination.begin(),destination.end()); // make empty
	}
	return destination;
}

//---------------------------------------------------------------------------------------
std::string TRIMRIGHT(const std::string& source, const char* chars2remove)
{
	if(source.empty())
	{
		return "";
	}
	std::string destination=source;
	std::string::size_type pos = destination.find_last_not_of(chars2remove);
	if(pos != std::string::npos)
	{
		destination.erase(pos+1);
	}
	else
	{
		destination.erase(destination.begin(),destination.end()); // make empty
	}
	return destination;
}

//---------------------------------------------------------------------------------------
std::string TRIMSPACE(const std::string& source)
{
	return TRIMRIGHT(TRIMLEFT(source," ")," ");
}

//---------------------------------------------------------------------------------------
std::string LONG2STR(long value)
{
	char tmp[100];
	sprintf(tmp,"%ld",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string ULONG2STR(unsigned int value)
{
	char tmp[100];
	sprintf(tmp,"%u",value);
	return tmp;
}


//---------------------------------------------------------------------------------------
std::string INT2STR(int value)
{
	char tmp[100];
	sprintf(tmp,"%d",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string UINT2STR(unsigned int value)
{
	char tmp[100];
	sprintf(tmp,"%u",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string SHORT2STR(short value)
{
	char tmp[100];
	sprintf(tmp,"%d",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string USHORT2STR(unsigned short value)
{
	char tmp[100];
	sprintf(tmp,"%u",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string BYTE2STR(unsigned char value)
{
	char tmp[100];
	sprintf(tmp,"%d",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
std::string BOOL2STR(bool value)
{
	return value?"true":"false";
}

//---------------------------------------------------------------------------------------
std::string DOUBLE2STR(double value)
{
	char tmp[100];
	sprintf(tmp,"%.10f",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
long STR2LONG(const std::string& value)
{
	return strtol(value.c_str(),NULL,10);
}

//---------------------------------------------------------------------------------------
unsigned int STR2ULONG(const std::string& value)
{
	return strtoul(value.c_str(),NULL,10);
}

//---------------------------------------------------------------------------------------
int STR2INT(const std::string& value)
{
	return atoi(value.c_str());
}

//---------------------------------------------------------------------------------------
unsigned int STR2UINT(const std::string& value)
{
	return atoi(value.c_str());
}

//---------------------------------------------------------------------------------------
short STR2SHORT(const std::string& value)
{
	return atoi(value.c_str());
}

//---------------------------------------------------------------------------------------
unsigned short STR2USHORT(const std::string& value)
{
	return atoi(value.c_str());
}

//---------------------------------------------------------------------------------------
unsigned char STR2BYTE(const std::string& value)
{
	return atoi(value.c_str());
}

//---------------------------------------------------------------------------------------
bool STR2BOOL(const std::string& value)
{
	return value=="true"?true:false;
}

//---------------------------------------------------------------------------------------
double STR2DOUBLE(const std::string& value)
{
	return atof(value.c_str());
}

//---------------------------------------------------------------------------------------
unsigned short BOOL2USHORT(bool value)
{
	return value?1:0;
}

//---------------------------------------------------------------------------------------
bool USHORT2BOOL(unsigned short value)
{
	return value==1?true:false;
}

//---------------------------------------------------------------------------------------
std::string ULONG2HEX(unsigned int value)
{
	char tmp[100];
    sprintf(tmp,"%X",value);
	return tmp;
}

//---------------------------------------------------------------------------------------
unsigned int HEX2ULONG(const std::string& value)
{
	unsigned int ulValue=0;
    sscanf(value.c_str(),"%X",&ulValue);
	return ulValue;
}

//---------------------------------------------------------------------------------------
std::string NONULLSTR(const char* value)
{
	return value==NULL?"":value;
}

//---------------------------------------------------------------------------------------
STRING_MAP StrSplit(const std::string& data,const std::string& token)
{
	STRING_MAP mapStringSplit;

//	int datalen=(int)data.length();
	int tokenlen=(int)token.length();
	int start=0;
	int end=0;
	int off=0;
	int count=0;
	while(end>=0)
	{
		end=(int)data.find(token,start);
		off=end-start;
		std::string subdata=data.substr(start,off);
		if(!subdata.empty())
		{
			mapStringSplit.insert(STRING_MAP::value_type(count,subdata));
		}
		start=end+tokenlen;
		count++;
	}
	return mapStringSplit;
}

//---------------------------------------------------------------------------------------
STRING_MAP_EX StrSplitEx(const std::string& data,const std::string& token)
{
	STRING_MAP_EX mapStringSplit;
	STRING_MAP mapSegment = StrSplit(data,token);
	for(size_t i=0;i<mapSegment.size();i++)
	{
		std::string seg = mapSegment[i];
		STRING_MAP b = StrSplit(seg,"=");
		std::string name = b[0];
		std::string value = b[1];
		mapStringSplit.insert(STRING_MAP_EX::value_type(name,value));
	}
	return mapStringSplit;
}

//=======================================================================================
// 命令字符串编解码
//=======================================================================================

//---------------------------------------------------------------------------------------
void CommandStringEncode(const std::string& in,std::string& out)
{
	for(size_t i=0;i<in.length();i++)
	{
		std::string temp;
		switch(in[i])
		{
		case ' ':
			temp = "&space;";
			break;
		case '=':
			temp = "&equal;";
			break;
		case '&':
			temp = "&amp;";
			break;
		case '\r':
			temp = "&return;";
			break;
		case '\n':
			temp = "&newline;";
			break;
		default:
			temp.append(1,in[i]);
		}
		out+=temp;
	}
}

//---------------------------------------------------------------------------------------
void CommandStringDecode(const std::string& in,std::string& out)
{
	out=in;
	while(true)
	{
		std::basic_string <char>::size_type index;
		index=out.find("&space;");
		if(index!= out.npos)
		{
			out.replace(index,7," ");
		}
		else
		{
			break;
		}
	}


	while(true)
	{
		std::basic_string <char>::size_type index;
		index=out.find("&equal;");
		if(index!= out.npos)
		{
			out.replace(index,7,"=");
		}
		else
		{
			break;
		}
	}

	while(true)
	{
		std::basic_string <char>::size_type index;
		index=out.find("&amp;");
		if(index!= out.npos)
		{
			out.replace(index,5,"&");
		}
		else
		{
			break;
		}
	}

	while(true)
	{
		std::basic_string <char>::size_type index;
		index=out.find("&return;");
		if(index != out.npos)
		{
			out.replace(index,8,"\r");
		}
		else
		{
			break;
		}
	}

	while(true)
	{
		std::basic_string <char>::size_type index;
		index=out.find("&newline;");
		if(index != out.npos)
		{
			out.replace(index,9,"\n");
		}
		else
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------------------
KCmdItem::KCmdItem(void)
{
}

//---------------------------------------------------------------------------------------
KCmdItem::KCmdItem(const std::string& data)
{
	STRING_MAP element=StrSplit(data," ");
	for(STRING_MAP::iterator it=element.begin();it!=element.end();it++)
	{
		std::string strElement=it->second;
		if(strElement.empty())
		{
			continue;
		}
		STRING_MAP attrib=StrSplit(strElement,"=");
		std::string name  = attrib[0];
		std::string value = attrib[1];
		m_mapAttribs[name] = value;
	}
}

//---------------------------------------------------------------------------------------
KCmdItem::KCmdItem(KCmdItem& rCmdItem)
{
	for(CMD_ATTRIB_MAP::iterator it=rCmdItem.m_mapAttribs.begin();it!=rCmdItem.m_mapAttribs.end();it++)
	{
		std::string name = it->first;
		std::string value = it->second;
		m_mapAttribs[name]=value;
	}
}

//---------------------------------------------------------------------------------------
KCmdItem::~KCmdItem(void)
{
	m_mapAttribs.clear();
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttrib(const std::string& name,const std::string& value)
{
	if(value=="" || value.empty())
	{
		return;
	}
	std::string _name = UPPERCASE(name);
	std::string _value = "";
	CommandStringEncode(value,_value);
	m_mapAttribs[_name] = _value;
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribDBL(const std::string& name,double value)
{
    if(value)
    {
        SetAttrib(name,DOUBLE2STR(value));
    }
}
//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribUL(const std::string& name,unsigned int value)
{
	if(value)
	{
		SetAttrib(name,ULONG2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribUN(const std::string& name,unsigned int value)
{
	if(value)
	{
		SetAttrib(name,UINT2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribUS(const std::string& name,unsigned short value)
{
	if(value)
	{
		SetAttrib(name,USHORT2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribUC(const std::string& name,unsigned char value)
{
	if(value)
	{
		SetAttrib(name,BYTE2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdItem::SetAttribBL(const std::string& name,bool value)
{
	if(value)
	{
		SetAttrib(name,"true");
	}
}

//---------------------------------------------------------------------------------------
KString KCmdItem::GetAttrib(const std::string& name)
{
	std::string _name = UPPERCASE(name);
	std::string _value = "";
	CommandStringDecode(m_mapAttribs[_name],_value);
	return _value;
}

//---------------------------------------------------------------------------------------
std::string KCmdItem::GetString(void)
{
	std::string attribs="";
	for(CMD_ATTRIB_MAP::iterator it=m_mapAttribs.begin();it!=m_mapAttribs.end();it++)
	{
		std::string name = it->first;
		std::string value = it->second;
		attribs += " " + name + "=" + value;
	}
	return TRIMSPACE(attribs);
}

//=======================================================================================
// 命令包类定义
//=======================================================================================

//---------------------------------------------------------------------------------------
KCmdPacket::KCmdPacket(const std::string& xns,const std::string& cmd,const std::string& userid)
:m_strXNS(xns)
,m_strCMD(cmd)
,m_strUserID(userid)
{
}

//---------------------------------------------------------------------------------------
KCmdPacket::KCmdPacket(const char* pData,int nLen)
:m_strXNS("ERR")
{
	if(nLen<=1 || pData==NULL || *(pData+nLen-1)!='\0')
	{
		return;
	}

	std::string strData(pData);
	STRING_MAP line=StrSplit(strData,"\r\n");
	if(line.size()==0)
	{
		printf("[KBASE] invalid command packet format(errno=1).\n");
		return;
	}
	//获取第一行
	std::string strFirstLine = line[0];

	//提取命令
	STRING_MAP element=StrSplit(strFirstLine," ");
	if(element.size()==0)
	{
		printf("[KBASE] invalid command packet format(errno=2).\n");
		return;
	}
	//获取XNS
	m_strXNS = element[0];

	//提取命令行属性
	int nAttribCount = element.size()-1;
	for(int i=1;i<=nAttribCount;i++)
	{
		std::string strElement=element[i];
		if(strElement.empty())
		{
			continue;
		}
		STRING_MAP attrib=StrSplit(strElement,"=");
		std::string name  = attrib[0];
		std::string value = attrib[1];
		std::string _name = UPPERCASE(name);

		if(_name=="CMD")
		{
			m_strCMD = value;
		}
		else if(_name=="USERID")
		{
			m_strUserID = value;
		}
		else
		{
			m_mapAttribs[_name] = value;
		}
	}

	//获取其它项
	int nLineCount = line.size()-1;
	for(int i=1;i<=nLineCount;i++)
	{
		std::string strElement=line[i];
		if(strElement.empty())
		{
			continue;
		}
		SetItem(strElement);
	}
}

//---------------------------------------------------------------------------------------
KCmdPacket::KCmdPacket(KCmdPacket& rCmdElement)
:m_strXNS(rCmdElement.GetXNS())
,m_strCMD(rCmdElement.GetCMD())
,m_strUserID(rCmdElement.GetUserID())
{
	for(CMD_ATTRIB_MAP::iterator it=rCmdElement.m_mapAttribs.begin();it!=rCmdElement.m_mapAttribs.end();it++)
	{
		std::string name = it->first;
		std::string value = it->second;
		m_mapAttribs[name]=value;
	}
	m_strItems = rCmdElement.GetItemString();
}

//---------------------------------------------------------------------------------------
KCmdPacket::~KCmdPacket(void)
{
	m_mapAttribs.clear();
	ClearAllItems();
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetXNS(const std::string& xns)
{
	m_strXNS = xns;
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetCMD(const std::string& cmd)
{
	m_strCMD = cmd;
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetUserID(const std::string& userid)
{
	m_strUserID = userid;
}

//---------------------------------------------------------------------------------------
std::string KCmdPacket::GetXNS(void)
{
	return m_strXNS;
}

//---------------------------------------------------------------------------------------
std::string KCmdPacket::GetCMD(void)
{
	return m_strCMD;
}

//---------------------------------------------------------------------------------------
std::string KCmdPacket::GetUserID(void)
{
	return m_strUserID;
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetItem(const std::string& item)
{
	m_strItems += item + "\r\n";
}

//---------------------------------------------------------------------------------------
std::string KCmdPacket::GetItemString(void)
{
	return m_strItems;
}

//---------------------------------------------------------------------------------------
CMD_ITEM_LST& KCmdPacket::GetItemList(void)
{
	m_lstItems.clear();
	STRING_MAP line=StrSplit(m_strItems,"\r\n");
	int nCount = line.size();
	for(int i=0;i<nCount;i++)
	{
		std::string strElement=line[i];
		if(strElement.empty())
		{
			continue;
		}
		m_lstItems.push_back(strElement);
	}
	return m_lstItems;
}

//---------------------------------------------------------------------------------------
void KCmdPacket::ClearAllItems(void)
{
	m_strItems = "";
	m_lstItems.clear();
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttrib(const std::string& name,const std::string& value)
{
	if(value=="" || value.empty())
	{
		return;
	}

	std::string _name = UPPERCASE(name);
	std::string _value = "";
	CommandStringEncode(value,_value);

	if(_name=="CMD")
	{
		m_strCMD = _value;
	}
	else if(_name=="USERID")
	{
		m_strUserID = _value;
	}
	else
	{
		m_mapAttribs[_name] = _value;
	}
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribDBL(const std::string& name,double value)
{
    if(value)
    {
        SetAttrib(name,DOUBLE2STR(value));
    }
}


//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribUL(const std::string& name,unsigned int value)
{
	if(value)
	{
		SetAttrib(name,ULONG2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribUN(const std::string& name,unsigned int value)
{
	if(value)
	{
		SetAttrib(name,UINT2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribUS(const std::string& name,unsigned short value)
{
	if(value)
	{
		SetAttrib(name,USHORT2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribUC(const std::string& name,unsigned char value)
{
	if(value)
	{
		SetAttrib(name,BYTE2STR(value));
	}
}

//---------------------------------------------------------------------------------------
void KCmdPacket::SetAttribBL(const std::string& name,bool value)
{
	if(value)
	{
		SetAttrib(name,"true");
	}
}

//---------------------------------------------------------------------------------------
KString KCmdPacket::GetAttrib(const std::string& name)
{
	std::string _name = UPPERCASE(name);
	std::string _value = "";

	if(_name=="CMD")
	{
		CommandStringDecode(m_strCMD,_value);
	}
	else if(_name=="USERID")
	{
		CommandStringDecode(m_strUserID,_value);
	}
	else
	{
		CommandStringDecode(m_mapAttribs[_name],_value);
	}
	return _value;
}

//---------------------------------------------------------------------------------------
std::string KCmdPacket::GetString(void)
{
	//获取命令包字符串
	std::string strAttribs = m_strXNS;
	strAttribs += " CMD=" + m_strCMD;
	if(!m_strUserID.empty())
	{
		strAttribs += " USERID=" + m_strUserID;
	}

	for(CMD_ATTRIB_MAP::iterator it=m_mapAttribs.begin();it!=m_mapAttribs.end();it++)
	{
		std::string name = it->first;
		std::string value = it->second;
		strAttribs += " " + name + "=" + value;
	}
	strAttribs = TRIMSPACE(strAttribs) + "\r\n";

	//获取项目字符串
	strAttribs += m_strItems;

	//返回
	return TRIMSPACE(strAttribs);
}

