#ifndef __WS_UTILS_STRING_H__
#define __WS_UTILS_STRING_H__

#include <vector>
#include <string>

namespace ws
{
	namespace utils
	{
		class String
		{
		public:
			/************************************************************************/
			/* ��seperator�ָ�str�������������output�������޸�str                    */
			/************************************************************************/
			static void split(char* str, const char* seperator, std::vector<char*>& output);
			/************************************************************************/
			/* ��seperator�ָ�str�������������output�������޸�str                    */
			/************************************************************************/
			static void split(const char* str, const char* seperator, std::vector<std::string>& output);

			static void toLowercase(char* str);
			static void toUppercase(char* str);
			/************************************************************************/
			/* �ж��ַ����Ƿ�Ϊ�ɴ�ӡASCII��                                          */
			/************************************************************************/
			static bool isPrintableString(const char* str);
			/************************************************************************/
			/* ��һ��ʱ���ַ�����ʽ��Ϊunix timestamp����ʽΪyyyy/mm/dd hh:mm:ss      */
			/************************************************************************/
			static time_t formatTime(const std::string& time);
			/************************************************************************/
			/* ��һ��unix timestampת��Ϊʱ���ַ�������ʽΪyyyy/mm/dd hh:mm:ss        */
			/************************************************************************/
			static std::string formatTime(time_t time = 0);

			static std::string md5(const std::string input);

			static std::string URLEncode(const std::string& input);
			static std::string URLDecode(const std::string& input);
			/************************************************************************/
			/* ��������ַ�����lengthָ�����ȣ�charsָ��ȡֵ���ݣ�Ĭ��Ϊ��Сд��ĸ������ */
			/************************************************************************/
			static std::string random(unsigned char length, const char* chars = nullptr);

		private:
			static std::string Bin2Hex(std::string input);
			static char Char2Hex(const char& input);
			static char Hex2Char(const char& input);
		};
	}
}

#endif