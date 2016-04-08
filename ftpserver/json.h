#pragma once
#include <vector>
//#define RAPIDJSON_HAS_STDSTRING  1
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h" 
#include "rapidjson/writer.h" 
#include "rapidjson/stringbuffer.h" 
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "rapidjson/encodedstream.h"


const int g_nMaxReadFileLen = 65536;
const int g_nMaxWriteFileLen = 65536;
//readme

//如要不破坏原有DOM的结构，需要进行深度拷贝
//Value.CopyFrom(vTemp, m_document.GetAllocator());
using namespace   rapidjson;
//如果节点名字有重复的，则程序会从中随机挑取一个返回，如果名字不存在，则返回false;  删除同理，
//所以请保证名字的唯一性,否则很可能得不到你想得到的结果。
//为方便阅读，直接将所有查询函数列出
//返回值支持如下几种类型
//int, unsigned, int64_t, uint64_t,double,float,string,bool,Vaule*

//获得根节点下指定项的值
//bool GetNodeValue(const char* pszName, T& retValue) 

//获得根节点下指定数组的值
//bool GetNodeArrayValue(const char* pszName, std::vector<T>& retValue);

//获得指定节点下指定项的值
//bool GetNodeValue(const char* pszName, const rapidjson::Value& vNode, T& retValue); 

//获得指定节点下指定数组的值
//bool GetNodeArrayValue(const char* pszName,const rapidjson::Value& vNode, std::vector<T>& retValue);

//从根节点开始查询，各个节点以\分割,如"root\department\section\office\zhangsan"  至少查询两个节点（一个节点请直接使用上面的函数）
//bool GetNodeValueByPath(const char* pszName, const rapidjson::Value& vNode, T& retValue); 

//bool GetNodeArrayValueByPath(const char* pszName, const rapidjson::Value& vNode, T& retValue); 

class CJson
{
public:
    CJson(void);
    virtual ~CJson(void);
    //读取
    //从字节流读取 
    bool ReadUtf8(const char* pchJson);
    bool ReadUtf16LE(const wchar_t* pchJson);
    bool ReadUtf16BE(const wchar_t* pchJson);
    bool ReadUtf32LE(const unsigned int* pchJson);
    bool ReadUtf32BE(const unsigned int*pchJson);
    //从文件读取
    bool ReadFromFile(const std::string& strPath);
    //查询
    template <typename T>
    bool GetNodeValue(const char* pszName, T& retValue) //目前支持int, unsigned, int64_t, uint64_t,double,string,bool,Vaule 八种类型，其余返回错误
    {
        if (m_document.FindMember(pszName) == m_document.MemberEnd())
        {
            return false;
        }
        Value& vTemp = m_document[pszName]; 
        return GetValue(vTemp, retValue);
    }


    template <typename T>
    bool GetNodeArrayValue(const char* pszName, std::vector<T>& retValue)
    {
        if (m_document.FindMember(pszName) == m_document.MemberEnd())
        {
            return false;
        }
        Value& vTemp = m_document[pszName];
        if (!vTemp.IsArray())
        {
            return false;
        }
        retValue.clear();
        int kTemp = vTemp.Size();
        for (SizeType	i	=	0; i	<	vTemp.Size();	i++)
        {
            Value& vArrayTemp = vTemp[i];
            T retTempValue;
            if (GetValue(vArrayTemp, retTempValue))
            {
                retValue.push_back(retTempValue);
            }
            else
            {
                retValue.clear();
                return false;
            }
        }
        return true;
    }

    template <typename T>
    bool GetNodeValue(const char* pszName, T& retValue,  rapidjson::Value* pNode)  //目前支持int, unsigned, int64_t, uint64_t,double,string,bool，Vaule 八种类型，其余返回错误
    {
        if (pNode->FindMember(pszName) == pNode->MemberEnd())
        {
            return false;
        }
        Value& vTemp = (*pNode)[pszName]; 
        return GetValue(vTemp, retValue);
    }
    template <typename T>
    bool GetNodeArrayValue(const char* pszName, std::vector<T>& retValue,  rapidjson::Value* pNode)
    {
        if (pNode->FindMember(pszName) == pNode->MemberEnd())
        {
            return false;
        }
        Value& vTemp = (*pNode)[pszName];
        if (!vTemp.IsArray())
        {
            return false;
        }
        retValue.clear();
        for (SizeType	i	=	0; i	<	vTemp.Size();	i++)
        {
            Value& vArrayTemp = vTemp[i];
            T retTempValue;
            if (GetValue(vArrayTemp, retTempValue))
            {
                retValue.push_back(retTempValue);
            }
            else
            {
                retValue.clear();
                return false;
            }
        }
        return true;
    }

    //从根节点开始，各个节点以\分割,如"root\department\section\office\zhangsan"  ,第一个节点必须是object
    template <typename T>
    bool GetNodeValueByPath(const char* pszName, T& retValue)
    {
        string strTemp = pszName;
        string strSplit = "\\\\";
        vector<string> vtTemp;
        vtTemp = stringsplit(strTemp , strSplit);
        if (0 == (int)vtTemp.size() )
        {
            return false;
        }
        string strName = vtTemp[0];
        if (m_document.FindMember(strName.c_str()) == m_document.MemberEnd())
        {
            return false;
        }
        int nSize = (int)vtTemp.size();
        Value* pVTemp = &m_document[strName.c_str()];
        if (1 == nSize)
        {
            return  GetValue(*pVTemp, retValue);
        }
        for (int i = 1; i < (int)vtTemp.size() - 1; ++i )
        {
            if(!pVTemp->IsObject())
            {
                return false;
            }
            strName = vtTemp[i];
            if (pVTemp->FindMember(strName.c_str()) == pVTemp->MemberEnd())
            {
                return false;
            }
            pVTemp = &(*pVTemp)[strName.c_str()];
        }
        //查找最后一个
        strName = vtTemp[(int)vtTemp.size() - 1];
        if (pVTemp->FindMember(strName.c_str()) == pVTemp->MemberEnd())
        {
            return false;
        }
        pVTemp = &(*pVTemp)[strName.c_str()];
        return  GetValue(*pVTemp, retValue);
    }

    template <typename T>
    bool GetNodeArrayValueByPath(const char* pszName, std::vector<T>& retValue)
    {
        string strTemp = pszName;
        string strSplit = "\\\\";
        vector<string> vtTemp;
        vtTemp = stringsplit(strTemp , strSplit);
        if (0 == (int)vtTemp.size() )
        {
            return false;
        }
        string strName = vtTemp[0];
        if (m_document.FindMember(strName.c_str()) == m_document.MemberEnd())
        {
            return false;
        }
        int nSize = (int)vtTemp.size();
        Value* pVTemp = &m_document[strName.c_str()];
        if (1 == nSize)
        {
            if (!(*pVTemp).IsArray())
            {
                return false;
            }
            retValue.clear();
            for (SizeType	i	=	0; i	<	(*pVTemp).Size();	i++)
            {
                Value& vArrayTemp = (*pVTemp)[i];
                T retTempValue;
                if (GetValue(vArrayTemp, retTempValue))
                {
                    retValue.push_back(retTempValue);
                }
                else
                {
                    retValue.clear();
                    return false;
                }
            }
            return  true;
        }
        for (int i = 1; i < (int)vtTemp.size() - 1; ++i )
        {
            if(!pVTemp->IsObject())
            {
                return false;
            }
            strName = vtTemp[i];
            if (pVTemp->FindMember(strName.c_str()) == pVTemp->MemberEnd())
            {
                return false;
            }
            pVTemp = &(*pVTemp)[strName.c_str()];
        }
        //查找最后一个
        strName = vtTemp[(int)vtTemp.size() - 1];
        if (pVTemp->FindMember(strName.c_str()) == pVTemp->MemberEnd())
        {
            return false;
        }
        pVTemp = &(*pVTemp)[strName.c_str()];
        if (!(*pVTemp).IsArray())
        {
            return false;
        }
        retValue.clear();
        for (SizeType	i	=	0; i	<	(*pVTemp).Size();	i++)
        {
            Value& vArrayTemp = (*pVTemp)[i];
            T retTempValue;
            if (GetValue(vArrayTemp, retTempValue))
            {
                retValue.push_back(retTempValue);
            }
            else
            {
                retValue.clear();
                return false;
            }
        }
        return  true;
    }
	
    //输出到string
    bool OutputToString(std::string& str);//将当前从流读入的内容输出到string，目前采用UTF-8的形式

	 //输出到fule
    bool OutputToFile(const std::string& strPath);//将当前从流读入的内容输出到文件，目前采用UTF-8的形式



private:
    void PrintPhraseError();
    bool GetValue(rapidjson::Value& vTemp, int& retValue);
    bool GetValue(rapidjson::Value& vTemp, double& retValue);
    bool GetValue(rapidjson::Value& vTemp, std::string& retValue);
    bool GetValue(rapidjson::Value& vTemp, bool& retValue);
    bool GetValue(rapidjson::Value& vTemp, int64_t& retValue);
    bool GetValue(rapidjson::Value& vTemp, unsigned& retValue);
    bool GetValue(rapidjson::Value& vTemp, uint64_t& retValue);
    bool GetValue(rapidjson::Value& vTemp, Value*& retValue);


	//增加，删除，修改
    bool Check();//检查DOM同一层是否有同名的节点，

    rapidjson::Document m_document;
  

//	static char m_scMaxFileReadBuffer[g_nMaxReadFileLen];
//	static char m_scMaxFileWriteBuffer[g_nMaxWriteFileLen];
};

//char CJson::m_scMaxFileReadBuffer[g_nMaxReadFileLen];
//char CJson::m_scMaxFileWriteBuffer[g_nMaxWriteFileLen];


