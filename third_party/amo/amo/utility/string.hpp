#ifndef amo_utility_string_h__
#define amo_utility_string_h__
#include <string>
#include <amo/config.hpp>
#include <amo/array.hpp>
#include <amo/base64.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _WIN32
//namespace amo  { namespace str{ 
//
//
//	static amo::array<int8_t, 4> intToBytes(int n)
//	{
//		amo::array<int8_t, 4> b;
//
//		for(int i = 0;i < 4;i++){
//			b[i] = (int8_t)(n >> (24 - i * 8)); 
//		}
//		return b;
//	}
//
//	static int bytesToInt(int8_t* b)
//	{
//		int mask = 0x000000ff; 
//		return ( (b[0]&mask)<< 24) + ((b[1]&mask) << 16) + ((b[2]&mask) << 8) + (b[3]&mask);
//	}
//
//	static int bytesToInt(amo::array<int8_t, 4>& b)
//	{
//		int mask = 0x000000ff; 
//		return ( (b[0]&mask)<< 24) + ((b[1]&mask) << 16) + ((b[2]&mask) << 8) + (b[3]&mask);
//	}
//
//	static std::string UTF8_To_string(const std::string & str)
//	{
//		$windows({
//			int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
//
//			wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
//			memset(pwBuf, 0, nwLen * 2 + 2);
//
//			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);
//
//			int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
//
//			char * pBuf = new char[nLen + 1];
//			memset(pBuf, 0, nLen + 1);
//
//			WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
//
//			std::string retStr = pBuf;
//
//			delete[]pBuf;
//			delete[]pwBuf;
//
//			pBuf = NULL;
//			pwBuf = NULL;
//
//			return retStr;
//		})
//			return "";
//	}
//
//	static std::string string_To_UTF8(const std::string & str)
//	{
//		$windows({
//			int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
//
//			wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
//			ZeroMemory(pwBuf, nwLen * 2 + 2);
//
//			::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
//
//			int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
//
//			char * pBuf = new char[nLen + 1];
//			ZeroMemory(pBuf, nLen + 1);
//
//			::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
//
//			std::string retStr(pBuf);
//
//			delete[]pwBuf;
//			delete[]pBuf;
//
//			pwBuf = NULL;
//			pBuf = NULL;
//
//			return retStr;
//		})
//		
//	}
//
//
//	static std::string WChar2Ansi(LPCWSTR pwszSrc)
//	{
//		$windows({
//			int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
//
//			if (nLen <= 0) return std::string("");
//
//			char* pszDst = new char[nLen];
//			if (NULL == pszDst) return std::string("");
//
//			WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
//			pszDst[nLen - 1] = 0;
//
//			std::string strTemp(pszDst);
//			delete[] pszDst;
//
//			return strTemp;
//		})
//			return "";
//	}
//
//	/*static std::string wstring_to_string(const wchar_t& ch)
//	{
//	std::wstring inputws(ch);
//	return WChar2Ansi(inputws.c_str());
//	}
//	*/
//	static std::string wstring_to_string(const std::wstring& inputws)
//	{
//		return WChar2Ansi(inputws.c_str());
//	}
//
//
//
//	static std::string wstring_to_string(const wchar_t* inputws)
//	{
//		return WChar2Ansi(inputws);
//	}
//
//	static std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen)
//	{
//		$windows(
//			int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
//		if (nSize <= 0) return NULL;
//
//		WCHAR *pwszDst = new WCHAR[nSize + 1];
//		if (NULL == pwszDst) return NULL;
//
//		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
//		pwszDst[nSize] = 0;
//
//		if (pwszDst[0] == 0xFEFF) // skip Oxfeff
//			for (int i = 0; i < nSize; i++)
//				pwszDst[i] = pwszDst[i + 1];
//
//		std::wstring wcharString(pwszDst);
//		delete pwszDst;
//
//		return wcharString;
//		)
//		return L"";
//
//	}
//
//	static std::wstring string_to_wstring(const std::string& s)
//	{
//		return Ansi2WChar(s.c_str(), s.size());
//	}
//
//	static std::string unicode_packet(const std::string& str)
//	{
//
//		return amo::base64::encode(string_To_UTF8(str));
//	}
//
//	static std::string unicode_unpacket(const std::string& str)
//	{
//		//return "";
//		return amo::string(amo::base64::decode(str), true).to_ansi();
//	}
//
//	
//
//	
//
//	static bool is_utf8_string(const char* str, ULONGLONG length)
//	{
//		$windows(
//		{
//			int i;
//			DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
//			UCHAR chr;
//			BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
//			for (i = 0; i < length; i++)
//			{
//				chr = *(str + i);
//				if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
//					bAllAscii = FALSE;
//				if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
//				{
//					if (chr >= 0x80)
//					{
//						if (chr >= 0xFC && chr <= 0xFD)
//							nBytes = 6;
//						else if (chr >= 0xF8)
//							nBytes = 5;
//						else if (chr >= 0xF0)
//							nBytes = 4;
//						else if (chr >= 0xE0)
//							nBytes = 3;
//						else if (chr >= 0xC0)
//							nBytes = 2;
//						else
//						{
//							return FALSE;
//						}
//						nBytes--;
//					}
//				}
//				else //多字节符的非首字节,应为 10xxxxxx
//				{
//					if ((chr & 0xC0) != 0x80)
//					{
//						return FALSE;
//					}
//					nBytes--;
//				}
//			}
//			if (nBytes > 0) //违返规则
//			{
//				return FALSE;
//			}
//			if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
//			{
//				return FALSE;
//			}
//			return TRUE;
//		})
//			return true;
//	}
//
//	static bool is_utf8_string(std::string str)
//	{
//		return is_utf8_string(str.data(), str.size());
//	}
//
//	static std::vector<int8_t> string_to_bytes(const std::string& str)
//	{
//		std::vector<int8_t> vec;
//		std::copy(str.begin(), str.end(), std::back_inserter(vec));
//		return vec;
//	}
//
//
//	static std::vector<std::string> split(std::string split_str, std::string split_char)
//	{
//		std::vector<std::string> vec_str;
//		boost::split(vec_str, split_str, boost::is_any_of(split_char));
//		return vec_str;
//	}
//
//	//指定次数
//	static std::vector<std::string> split(std::string split_str, std::string split_char, unsigned int num)
//	{
//		std::vector<std::string> vec_str = split(split_str, split_char);
//		std::vector<std::string> vec_rec;
//
//
//		if (vec_str.size() > num)
//		{
//			for (size_t i = 0; i < vec_str.size(); ++i)
//			{
//				if (i < num)
//					vec_rec.push_back(vec_str.at(i));
//				else
//				{
//					vec_rec.at(num - 1) = vec_rec.at(num - 1) + ":" + vec_str.at(i);
//				}
//			}
//			return vec_rec;
//		}
//		return vec_str;
//	}
//
//
//}  }
#endif

#endif // string_h__
