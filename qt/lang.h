#pragma once

#include "qstring.h"
#include "string"


// 语言定义
#define ZH "zh"
#define EN "en"

// code定义
#define TITLE "title"


class lang_util
{
private:
	static std::string lang;
public:

	static QString get_by_code(std::string code);

	static void set_lang(std::string code);
};

