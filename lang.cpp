#include "lang.h"
#include <map>


std::string lang_util::lang;

std::map<std::string, std::map<std::string, QString>> lang_map = {
	{ZH, {{TITLE, QString::fromUtf8("密码管理")}}},
	{EN, {{TITLE, QString::fromUtf8("Password-Manager")}}}
};

QString lang_util::get_by_code(std::string code) {
	return lang_map[lang_util::lang][code];
}


void lang_util::set_lang(std::string _lang) {
	lang_util::lang = _lang;
}