#include "common.h"
#include <codecvt>
#include <strstream>
#include <regex>
#include <boost/lexical_cast.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>

const std::wstring s2ws(const std::string& src)
{
	std::locale sys_locale("");

	const char* data_from = src.c_str();
	const char* data_from_end = src.c_str() + src.size();
	const char* data_from_next = 0;

	wchar_t* data_to = new wchar_t[src.size() + 1];
	wchar_t* data_to_end = data_to + src.size() + 1;
	wchar_t* data_to_next = 0;

	wmemset(data_to, 0, src.size() + 1);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t in_state = 0;
	auto result = std::use_facet<convert_facet>(sys_locale).in(
		in_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::wstring dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf("convert error!\n");
		delete[] data_to;
		return std::wstring(L"");
	}
}
const std::string ws2utf8(const std::wstring& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(src);
}
const std::string s2utf8(const std::string& src)
{
	return ws2utf8(s2ws(src));
}
const std::string ws2s(const std::wstring& src)
{
	std::locale sys_locale("");

	const wchar_t* data_from = src.c_str();
	const wchar_t* data_from_end = src.c_str() + src.size();
	const wchar_t* data_from_next = 0;

	int wchar_size = 4;
	char* data_to = new char[(src.size() + 1) * wchar_size];
	char* data_to_end = data_to + (src.size() + 1) * wchar_size;
	char* data_to_next = 0;

	memset(data_to, 0, (src.size() + 1) * wchar_size);

	typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
	mbstate_t out_state = 0;
	auto result = std::use_facet<convert_facet>(sys_locale).out(
		out_state, data_from, data_from_end, data_from_next,
		data_to, data_to_end, data_to_next);
	if (result == convert_facet::ok)
	{
		std::string dst = data_to;
		delete[] data_to;
		return dst;
	}
	else
	{
		printf("convert error!\n");
		delete[] data_to;
		return std::string("");
	}
}
const std::wstring utf8_2_ws(const std::string& src)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(src);
}
const std::string utf8_2_s(const std::string& str)
{
	return ws2s(utf8_2_ws(str));
}
bool StringToInt(const std::string& str, int& n)
{
	try
	{
		n = boost::lexical_cast<int>(str);
	}
	catch (boost::bad_lexical_cast & e)
	{
		printf("%s/r/n", e.what());
		return false;
	}

	return true;
}
std::vector<std::string> stringsplit(std::string strSrc, const std::string& split)
{
	std::regex reg(split);
	std::vector<std::string> vec;
	std::sregex_token_iterator pos(strSrc.cbegin(), strSrc.cend(), reg, -1);
	std::sregex_token_iterator end;
	for (; pos != end; ++pos)
	{
		vec.push_back(*pos);
	}
	return vec;
}


bool g_nInitLog = false;
namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
void InitLog()
{
	//输出到文件
	auto pSink = logging::add_file_log
		(
		keywords::open_mode = std::ios::app,  //重启程序不删除日志
		keywords::file_name = "log/ftpserver_%N.log",
		keywords::rotation_size = 10 * 1024 * 1024,       //超过此大小自动建立新文件
		// keywords::time_based_rotation=sinks::file::rotation_at_time_point(0,0,0),   //每隔指定时间重建新文件
		// This makes the sink to write log records that look like this:
		keywords::format =
		(
		expr::stream
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
		<< " " << expr::attr< boost::log::aux::thread::id >("ThreadID")
		<< ": <" << logging::trivial::severity
		<< "> " << expr::smessage
		)
		);
	pSink->locked_backend()->auto_flush(true);//使日志实时更新  
	//pSink->imbue(std::locale("zh_CN.UTF-8")); // 本地化   
	logging::add_console_log();
	logging::add_common_attributes();
}