// RapidJsonTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Json.h"
#include "common.h"
#include <iostream>


using namespace std;
void TestOutputJson();

using namespace std;
int main(int argc, char* argv[])
{
  //  OspInit(true, 2401);
    string strWait;
    cin >> strWait;
	TestOutputJson();

    //string strLogFileName = "JsonLog.txt";
    //if (!OspOpenLogFile(strLogFileName.c_str(), 1, 3))
    //{
    //    cout <<"Log file open failed"<<endl;
    //    OspPrintf(true, true, "Log file open failed \n");
    //}
    //OspPrintf(true, true, "Osp 初始化成功\n");
    CJson cTemp;
    string strPath1 = "D:\\TestData\\2json.txt";
    if(!cTemp.ReadFromFile(strPath1))
    {
        cout <<"read error" <<endl;
        return -1;
    }
    //得到名字1，名字2，整数
    string strUtf8 = ws2utf8(L"名字1");
    string strDst;  
    if (!cTemp.GetNodeValue(strUtf8.c_str(), strDst))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << utf8_2_s(strDst)  <<endl;
    strUtf8 = ws2utf8(L"名字2");
    if (!cTemp.GetNodeValue(strUtf8.c_str(), strDst))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << utf8_2_s(strDst)  <<endl;
    strUtf8 = ws2utf8(L"整数");
    int nDst;
    if (!cTemp.GetNodeValue(strUtf8.c_str(), nDst))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << nDst <<endl;

    //访问数组  
    strUtf8 = ws2utf8(L"数组1");
    std::vector<double> vtTemp;
    if(!cTemp.GetNodeArrayValue(strUtf8.c_str(), vtTemp))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }

    //访问节点
    strUtf8 = ws2utf8(L"节点1");
    Value *pNode;
    if(!cTemp.GetNodeValue(strUtf8.c_str(), pNode))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    //得到节点1的 名字11的数据
    strUtf8 = ws2utf8(L"名字11");
    if (!cTemp.GetNodeValue(strUtf8.c_str(), strDst, pNode))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << utf8_2_s(strDst)  <<endl;
    //得到节点1的节点11的节点
    strUtf8 = ws2utf8(L"节点1111");
    Value* pNode11;
    if(!cTemp.GetNodeValue(strUtf8.c_str(),pNode11, pNode))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    //操作其余节点的操作类似
    //直接从根操作某个节点的数据
    strUtf8 = ws2utf8(L"节点1\\节点1111\\名字111");
    string vTemp2;
    if (!cTemp.GetNodeValueByPath(strUtf8.c_str(), vTemp2))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << utf8_2_s(vTemp2)  <<endl;
    //获得数组内容
    strUtf8 = ws2utf8(L"节点1\\节点1111");
    Value* pNode111; 
    if (!cTemp.GetNodeValueByPath(strUtf8.c_str(), pNode111))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }

    strUtf8 = ws2utf8(L"节点1\\节点2");
    std::vector<string> vtstrTemp; 
    if (!cTemp.GetNodeArrayValueByPath(strUtf8.c_str(), vtstrTemp))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    for (int i = 0; i < (int)vtstrTemp.size(); ++i)
    {
        cout << utf8_2_s(vtstrTemp[i])  <<endl;
    }


    std::vector<Value *> vtVTemp;
    strUtf8 = ws2utf8(L"数组111");
    if (!cTemp.GetNodeArrayValue(strUtf8.c_str(), vtVTemp, pNode111))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    strUtf8 = ws2utf8(L"名字1111");
    if (!cTemp.GetNodeValue(strUtf8.c_str(),  strDst, vtVTemp[0]))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
    cout << utf8_2_s(strDst)  <<endl;

    strUtf8 = ws2utf8(L"数字1111");
    if (!cTemp.GetNodeValue(strUtf8.c_str(),  nDst, vtVTemp[0]))
    {
        cout <<"出错处理"<<endl;
        return -1;
    }
	

    cout << nDst  <<endl;
	cTemp.OutputToString(strUtf8);
	cout << utf8_2_s(strUtf8)  <<endl;
	//此处我添加一个节点

	rapidjson::Document doc1;
	doc1.SetObject();
	Value author;
	char buffer[10];
	int len = sprintf(buffer, "%s %s", "中国", "中国"); // 动态创建的字符串。
	author.SetString(buffer, len, doc1.GetAllocator());
	memset(buffer, 0, sizeof(buffer));
	doc1.AddMember ("aa", author, doc1.GetAllocator());
	//char *p1 = "aa";
	//const char *p2 = "bb";
	//doc1.AddMember (p2, author, doc1.GetAllocator());
	StringBuffer buffer2;
	Writer<StringBuffer> writer(buffer2);
	if (!doc1.Accept(writer))
	{
	//	OspPrintf(true, true, "转换字符流失败");
        return false;
	}
	string str2 = buffer2.GetString();
	cout<<str2<<endl;



	cout << nDst  <<endl;
	cTemp.OutputToString(strUtf8);
	cout << utf8_2_s(strUtf8)  <<endl;

    string strTemp2;
    cin>>strTemp2;

  //  OspCloseLogFile();
//    OspQuit();
    return 0;
   
    getchar(); 
	return 0;
}
void TestOutputJson()
{
	CJson cTemp;
	string strPath1 = "{\"名字2\": \"这个优先\"}";
//	string strUtf8 = ws2utf8(L"{\"名字2\": \"这个优先\"}");
    if(!cTemp.ReadUtf8(strPath1.c_str()))
    {
        cout <<"read error" <<endl;
        return;
    }
	
	cout << strPath1.c_str() <<endl;
	string strOut;
	cTemp.OutputToString(strOut);

	string strSavePath = "D:/TestData/1/a10.txt";
	FILE*	fp	=	fopen(strSavePath.c_str(),	"wb");	//	非Windows平台使用"w"
	if (!fp)
	{
//		OspPrintf(true, true, "文件%s打开失败",  strSavePath.c_str());
		return ;
	}
	fwrite(strOut.c_str(), 1, strOut.size(), fp);
	fclose(fp);



	//
	StringBuffer s;
	//Writer<StringBuffer> writer(s);
	PrettyWriter<StringBuffer> writer(s);
	writer.StartObject();
	std::string str1 = "hello";
	writer.Key(str1.c_str());
	writer.String("world");
	writer.Key("t");
	writer.Bool(true);
	writer.Key("f");
	writer.Bool(false);
	writer.Key("n");
	writer.Null();
	writer.Key("i");
	writer.Uint(123);
	writer.Key("pi");
	writer.Double(3.1416);
	writer.Key("a");
	writer.StartArray();
	for (unsigned i = 0; i < 4; i++)
		writer.Uint(i);
	writer.EndArray();

	writer.Key("节点11");
	writer.StartObject();
	writer.Key("数组3");
	writer.StartArray();
	for (unsigned i = 0; i < 4; i++)
	{
		writer.StartObject();
		char ch = 'a' + i;
		string strTemp, strTemp2;
		strTemp = "test 5";
		strTemp2 = "name 5";
		strTemp[5] = ch;
		writer.Key(strTemp2.c_str());
		writer.String(strTemp.c_str());
		writer.EndObject();
	}
	writer.EndArray();
	writer.EndObject();
	writer.EndObject();
	cout << s.GetString() << endl;
}