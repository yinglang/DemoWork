#define MYTOOL_MODEL
#ifdef MYTOOL_MODEL

#ifndef MYTOOL_H
#define MYTOOL_H
#include "cocos2d.h"   // 包括了 <sstream.h> 和 <string> 但没有 <iostream> 所以不能直接使用cout
#include <iostream>

// #define mycout std::cout 宏定义不能跨文件使用，只能在同一个类的.cpp和.h中使用

class MyTool
{
private :
	MyTool(){};
	
public:

	template <class Type>
	static std::string num2str(Type num){
		std::stringstream ss;
		ss << num;
		return ss.str();
	}

	static void log_example(){
		cocos2d::log("------------------------------------------------------------------------------------\n");
		cocos2d::log("the is my log example.\n ");
		log(126);
		log(126.0);
		cocos2d::log("the %dnd is c style printf", 2);
		cocos2d::log("------------------------------------------------------------------------------------\n");
	}

	static int sig(double num){  //  返回一个数的符号
		return (num > 0) ? 1 : -1;
	}

	template <class Type> // 使用该函数最好先用isDigit判断一下，否则对于非数字字符的转化会出问题
	static Type str2num(const std::string& str)	
	{
		std::istringstream iss(str);
		Type num;
		iss >> num;
		return num;
	}

	static bool isDigit(const std::string& str){		// 注意第一个为.2 和 +.2 和 -.2的字符串都返回false
		if (str.length() <= 0) return false;
		int i = 0;
		if (str[i] == '+' || str[i] == '-') i++;
		if (str.length() > 1 && str[i] == '.') return false;
		int dotCount = 0;						// 小数点的个数
		for (i; i < str.length(); i++){
			if (str[i] > '9' || str[i] < '0'){
				if (str[i] == '.') dotCount++;
				else return false;
			}
			if (dotCount > 1) return false;
		}
		return true;
	}
	
	static bool isInt(const std::string& str){
		if (str.length() <= 0) return false;
		int i = 0;
		if (str[i] == '+' || str[i] == '-') i++;
		for (; i < str.length(); i++){
			if (str[i] > '9' || str[i] < '0'){ 
				return false;
			}
		}
		return true;
	}

	static std::string int16Tostr0x(unsigned short num)//16bit的整数转化为16进制的str输出
	{
		const char hexbit[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
		std::string s;
		while (num != 0){
			unsigned short b = num % 16;
			num /= 16;
			s += hexbit[b];
		}
		std::string rs(s);
		for (int i = s.length() - 1; i >= 0; i--){
			rs[i] = s[s.length() - 1 - i];
		}
		return std::string("0x" + rs);
	}
};
#endif

#endif

