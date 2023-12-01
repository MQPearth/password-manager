#include "cloud.h"

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include "json.h"
#include "passwordmanager.h"


Json::Value cloud_util::get(passwordmanager* ui, QString& host, QString& login_name, QString& password) {
	QMessageBox msgBox(ui);

	msgBox.setWindowTitle("提示");
	msgBox.setText("正在拉取云端配置...");

	msgBox.setIcon(QMessageBox::Information);

	msgBox.setStandardButtons(QMessageBox::NoButton);

	msgBox.show();

	QNetworkRequest request;
	request.setUrl(QUrl(host + "/pwd/get"));

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

	QJsonObject jsonObject;
	jsonObject["login_name"] = login_name;
	jsonObject["password"] = password;

	QJsonDocument jsonDocument(jsonObject);
	QByteArray jsonData = jsonDocument.toJson();

	QNetworkReply* reply = ui->manager->post(request, jsonData);

	QTimer timer;
	timer.setSingleShot(true);
	timer.setInterval(5000);
	QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
	QObject::connect(reply, &QNetworkReply::finished, &timer, &QTimer::stop);
	timer.start();

	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	Json::Value json;

	try
	{
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();

			auto _txt = QString::fromUtf8(responseData);
			json = json_util::parse(_txt);

			reply->deleteLater();
			msgBox.close();
			return json;
		}
		else {
			qDebug() << "Error:" << reply->errorString();

			reply->deleteLater();
			msgBox.close();
			return json;
		}
	}
	catch (...)
	{
		msgBox.close();
		return json;
	}

}


Json::Value cloud_util::update(passwordmanager* ui, QString& host, QString& login_name, QString& password, QString& enc_data) {

	QMessageBox msgBox(ui);

	msgBox.setWindowTitle("提示");
	msgBox.setText("正在备份密码...");

	msgBox.setIcon(QMessageBox::Information);

	msgBox.setStandardButtons(QMessageBox::NoButton);

	msgBox.show();

	QNetworkRequest request;
	request.setUrl(QUrl(host + "/pwd/update"));

	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

	QJsonObject jsonObject;
	jsonObject["login_name"] = login_name;
	jsonObject["password"] = password;
	jsonObject["enc_data"] = enc_data;

	QJsonDocument jsonDocument(jsonObject);
	QByteArray jsonData = jsonDocument.toJson();

	QNetworkReply* reply = ui->manager->put(request, jsonData);

	QTimer timer;
	timer.setSingleShot(true);
	timer.setInterval(5000);
	QObject::connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
	QObject::connect(reply, &QNetworkReply::finished, &timer, &QTimer::stop);
	timer.start();

	QEventLoop loop;
	QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	Json::Value json;

	try
	{
		if (reply->error() == QNetworkReply::NoError) {
			QByteArray responseData = reply->readAll();

			auto _txt = QString::fromUtf8(responseData);
			json = json_util::parse(_txt);

			reply->deleteLater();
			msgBox.close();
			return json;
		}
		else {
			qDebug() << "Error:" << reply->errorString();

			reply->deleteLater();

			msgBox.close();
			return json;
		}
	}
	catch (...)
	{
		msgBox.close();
		return json;
	}
}