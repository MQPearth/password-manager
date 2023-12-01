#include "passwordmanager.h"
#include <QtWidgets/QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QStyleFactory>
#include "encrypt.h"
#include "file.h"
#include "json.h"
#include "cloud.h"
#include "message.h"
#include <QPushbutton>
#include <QThread>


QString get_password(bool exist, passwordmanager* w) {

	QString title;
	QString tips;
	QLineEdit::EchoMode echoMode;

	if (exist) {
		echoMode = QLineEdit::Password;
		title = "请输入密码";
	}
	else {
		echoMode = QLineEdit::Normal;
		title = "请设置密码";
		tips = "初次使用请设置密码";
	}

	QString text;

	while (true)
	{
		bool is_ok;
		text = QInputDialog::getText(w, title, tips, echoMode, "1234", &is_ok, Qt::WindowFlags());

		if (!is_ok) {
			qDebug() << "用户取消了输入";
			return text;
		}

		if (text.isEmpty()) {
			tips = "<p style='color: red'>密码不能为空</p>";
			continue;
		}
		else {
			if (text.length() < 4) {
				tips = "<p style='color: red'>密码不能小于四位</p>";
				continue;
			}
			break;
		}
	}

	return text;
}


int main(int argc, char* argv[])
{
	QApplication::setStyle(QStyleFactory::create("Fusion"));

	QApplication a(argc, argv);
	passwordmanager w;
	try {

		w.setWindowTitle("密码管理器");
		w.show();

		w.path = argv[0];

		bool local_exist = file_util::check_bin_file(w.path);
		bool server_exist = file_util::check_server_file(w.path);

		QString input_password = get_password(local_exist || server_exist, &w);
		qDebug() << input_password;
		if (input_password.isEmpty()) {
			return -1;
		}

		w.password = input_password;

		QString server_config_txt;
		Json::Value server_config;
		QString cloud_host;
		QString cloud_login_name;
		QString cloud_password;

		QString cloud_file_content;
		long long cloud_update_time = 0;
		long long local_update_time = 0;
		QString local_file_content;
		bool result;

		Json::Value data;

		if (server_exist) {
			server_config_txt = file_util::read_server(w.path, w.password);
			
			server_config = json_util::parse(server_config_txt);

			cloud_host = QString::fromStdString(server_config["host"].asString());
			cloud_login_name = QString::fromStdString(server_config["login_name"].asString());
			cloud_password = QString::fromStdString(server_config["password"].asString());
	
			auto cloud_config = cloud_util::get(&w, cloud_host, cloud_login_name, cloud_password);
			if (cloud_config["code"].asString() == "0") {
				cloud_file_content = encrypt_util::decrypt(QString::fromStdString(cloud_config["data"]["enc_data"].asString()), w.password);
				cloud_update_time = cloud_config["data"]["update_time"].asInt64();
			}
			else {
				auto err_msg = cloud_config["msg"].asString();
				server_exist = false;
				message_util::show(&w, QString::fromStdString(err_msg.empty() ? "网络错误" : err_msg));
			}
		}

		if (local_exist) {
			local_file_content = file_util::read(w.path, w.password);
			local_update_time = file_util::get_bin_file_last_modify_time(w.path);
		}


		if (local_exist && server_exist) {

			auto local_md5 = encrypt_util::md5(local_file_content);
			auto cloud_md5 = encrypt_util::md5(cloud_file_content);

			if (local_md5 != cloud_md5) {

				QMessageBox msgBox(&w);
				msgBox.setWindowTitle("本地和云端数据冲突");

				auto tips = QString(cloud_update_time > local_update_time ? "云端" : "本地");

				msgBox.setText(tips + "数据较新<br / > 请选择冲突解决方式");

				QPushButton* merge_button = msgBox.addButton("云端和本地合并", QMessageBox::ActionRole);
				QPushButton* local_button = msgBox.addButton("以本地为准", QMessageBox::ActionRole);
				QPushButton* cloud_button = msgBox.addButton("以云端为准", QMessageBox::ActionRole);

				msgBox.setDefaultButton(merge_button);
				msgBox.exec();

				if (msgBox.clickedButton() == merge_button) {
					auto local_data = json_util::parse(local_file_content);
					auto cloud_data = json_util::parse(cloud_file_content);

					local_data["data"].append(cloud_data["data"]);
					data = local_data;
				}
				else if (msgBox.clickedButton() == local_button) {
					data = json_util::parse(local_file_content);
				}
				else if (msgBox.clickedButton() == cloud_button) {
					data = json_util::parse(cloud_file_content);
				}
				
			}
			else {
				data = json_util::parse(local_file_content);
			}
		}
		else if (local_exist) {
			data = json_util::parse(local_file_content);
		}
		else if (server_exist) {
			data = json_util::parse(cloud_file_content);
		}
		else {
			message_util::show(&w, "<p>&nbsp;&nbsp;&nbsp;&nbsp;请牢记你的密码&nbsp;&nbsp;&nbsp;&nbsp;</p> <p>&nbsp;&nbsp;&nbsp;&nbsp;[" +
				w.password + "]&nbsp;&nbsp;&nbsp;&nbsp;</p><p style='color: red'>&nbsp;&nbsp;&nbsp;&nbsp;密码丢失即数据丢失&nbsp;&nbsp;&nbsp;&nbsp;</p>");
			local_file_content = R"({"verify": true, "data": []})";
			data = json_util::parse(local_file_content);
		}

		int code = w.init_data(data);

		if (code) {
			auto err_msg = "加密文件内容错误: " + local_file_content;
			throw std::runtime_error(err_msg.toStdString());
		}

		if (w.root["data"].size() == 0) {
			w.write_to_file();
		}

		return a.exec();
	}
	catch (const std::exception& e) {
		message_util::show(&w, e.what());
		return -2;
	}
	catch (...) {
		message_util::show(&w, "未知异常");
		return -1;
	}

}