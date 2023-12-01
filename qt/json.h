#pragma once


#include <jsoncpp/json.h>
#include <QString>

class json_util
{
public:
	static Json::Value parse(QString &txt);
};

