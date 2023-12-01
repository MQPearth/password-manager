#pragma once

#include <jsoncpp/json.h>
#include <QString>
#include <QWidget> 
#include <QThread> 
#include "passwordmanager.h"

class cloud_util
{

public:
	static Json::Value get(passwordmanager* manager, QString& host, QString& login_name, QString& password);

	static Json::Value update(passwordmanager* manager, QString& host, QString& login_name, QString& password, QString& enc_data);
};

