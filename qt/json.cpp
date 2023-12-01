#include "json.h"


Json::Value json_util::parse(QString& txt) {
	Json::Value value;

	Json::CharReaderBuilder reader;

	std::istringstream jsonStream(txt.toStdString());

	Json::parseFromStream(reader, jsonStream, &value, nullptr);

	return value;
}