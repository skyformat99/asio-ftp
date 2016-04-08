#include "json.h"

using namespace std;

CJson::CJson(void)
{
}


CJson::~CJson(void)
{
}
bool CJson::ReadUtf8(const char* pchJson)
{
    GenericStringStream<UTF8<> > sStream(pchJson);
    if (m_document.ParseStream(sStream).HasParseError())
    {
        //出错处理   
        PrintPhraseError();
        return false;
    }
    return true;
}
bool CJson::ReadUtf16LE(const wchar_t* pchJson)
{
    GenericStringStream<UTF16LE<> > sStream(pchJson);
    if (m_document.ParseStream(sStream).HasParseError())
    {
        //出错处理   
        PrintPhraseError();
        return false;
    }
    return true;
}
bool CJson::ReadUtf16BE(const wchar_t* pchJson)
{
    GenericStringStream<UTF16BE<> > sStream(pchJson);
    if (m_document.ParseStream(sStream).HasParseError())
    {
        //出错处理   
        PrintPhraseError();
        return false;
    }
    return true;
}
bool CJson::ReadUtf32LE(const unsigned int* pchJson)
{
    GenericStringStream<UTF32LE<> > sStream(pchJson);
    if (m_document.ParseStream(sStream).HasParseError())
    {
        //出错处理   
        PrintPhraseError();
        return false;
    }
    return true;
}
bool CJson::ReadUtf32BE(const unsigned int* pchJson)
{
    GenericStringStream<UTF32BE<> > sStream(pchJson);
    if (m_document.ParseStream(sStream).HasParseError())
    {
        //出错处理   
        PrintPhraseError();
        return false;
    }
    return true;
}




bool CJson::ReadFromFile(const string& strPath)
{
    FILE*	fp	=	fopen(strPath.c_str(),	"rb");	//	非Windows平台使用"r"
    if (!fp)
    {
//        OspPrintf(true, true, "文件%s打开失败",  strPath.c_str());
        return false;
    }

	char* pBuffer = new char[65536];

    FileReadStream	bis(fp,	pBuffer,	g_nMaxReadFileLen);

    AutoUTFInputStream<unsigned,	FileReadStream>	eis(bis);		//	用eis包装bis
    if (m_document.ParseStream<0,	AutoUTF<unsigned>	>(eis).HasParseError())	//	把任何UTF编码的文件解析至内存中的UTF-8
    {
        fclose(fp);
        //出错处理   
        PrintPhraseError();
		delete []pBuffer;
        return false;
    }
    fclose(fp);
	delete []pBuffer;
    return true;
}
//输出到string
bool CJson::OutputToString( std::string& str)//将当前从流读入的内容输出到string，目前采用UTF-8的形式
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	if (!m_document.Accept(writer))
	{
	//	OspPrintf(true, true, "转换字符流失败");
        return false;
	}
	str = buffer.GetString();
	return true;
}

bool CJson::OutputToFile(const std::string& strPath)
{
    FILE*	fp	=	fopen(strPath.c_str(),	"wb");	//	非Windows平台使用"w"
    if (!fp)
    {
     //   OspPrintf(true, true, "文件%s打开失败",  strPath.c_str());
        return false;
    }

	char* pBuffer = new char[65536];
    FileWriteStream	bos(fp,	pBuffer,	g_nMaxWriteFileLen);
    typedef	EncodedOutputStream<UTF8<>,	FileWriteStream>	OutputStream;
    OutputStream	eos(bos, true);
    Writer<OutputStream,	UTF8<>,	UTF8<>	>	writer(eos);
    if (!m_document.Accept(writer))
    {
        fclose(fp);
		delete []pBuffer;
//        OspPrintf(true, true, "文件% 准备写入时Accept失败",  strPath.c_str());
        return false;
    }
	delete []pBuffer;
    fclose(fp);
    return true;
}
void CJson::PrintPhraseError()
{
    string  strTemp =  GetParseError_En(m_document.GetParseError());
    int nK1 =  (unsigned)m_document.GetErrorOffset();
//     OspPrintf(true, true, "第%d个字符有错误,错误原因:%s \n", (unsigned)m_document.GetErrorOffset(), GetParseError_En(m_document.GetParseError() ) );
}
bool CJson::GetValue(rapidjson::Value& vTemp, int& retValue)
{
    if (!vTemp.IsInt())
    {
        return false;
    }
    retValue = vTemp.GetInt();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, double& retValue)
{
    if (!vTemp.IsDouble())
    {
        if (!vTemp.IsInt())
        {
            return false;
        }
        //
    }
    retValue = vTemp.GetDouble();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, string& retValue)
{
    if (!vTemp.IsString())
    {
        return false;
    }
    retValue = vTemp.GetString();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, bool& retValue)
{
    if (!vTemp.IsBool())
    {
        return false;
    }
    retValue = vTemp.GetBool();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, int64_t& retValue)
{
    if (!vTemp.IsInt64())
    {
        return false;
    }
    retValue = vTemp.GetInt64();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, unsigned& retValue)
{
    if (!vTemp.IsUint())
    {
        return false;
    }
    retValue = vTemp.GetUint();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, uint64_t& retValue)
{
    if (!vTemp.IsUint64())
    {
        return false;
    }
    retValue = vTemp.GetUint64();
    return true;
}
bool CJson::GetValue(rapidjson::Value& vTemp, Value*& retValue)
{
    if (!vTemp.IsObject())
    {
        return false;
    }
    retValue = &vTemp;
    return true;
}
