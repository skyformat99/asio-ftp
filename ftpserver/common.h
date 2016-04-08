#include <string>
#include <vector>
const std::string ws2s(const std::wstring& src);
const std::wstring s2ws(const std::string& src);
const std::string ws2utf8(const std::wstring& src);
const std::string s2utf8(const std::string& src);
const std::wstring utf8_2_ws(const std::string& src);
const std::string utf8_2_s(const std::string& str);
bool StringToInt(const std::string& str, int& n);
std::vector<std::string> stringsplit(std::string strSrc, const std::string& split);