#pragma once

#include "ui_passwordmanager.h"
#include <jsoncpp/json.h>
#include <QNetworkAccessManager>



#define CATEGORY "category"
#define URL "url"
#define LOGIN_NAME "login_name"
#define PASSWORD "password"
#define NOTE "note"

#define GET_CATEGORY(entry) entry[CATEGORY].asString()
#define GET_URL(entry) entry[URL].asString()
#define GET_LOGIN_NAME(entry) entry[LOGIN_NAME].asString()
#define GET_PASSWORD(entry) entry[PASSWORD].asString()
#define GET_NOTE(entry) entry[NOTE].asString()


#define GET_CATEGORY_Q(entry) QString::fromStdString(entry[CATEGORY].asString())
#define GET_URL_Q(entry) QString::fromStdString(entry[URL].asString())
#define GET_LOGIN_NAME_Q(entry) QString::fromStdString(entry[LOGIN_NAME].asString())
#define GET_PASSWORD_Q(entry) QString::fromStdString(entry[PASSWORD].asString())
#define GET_NOTE_Q(entry) QString::fromStdString(entry[NOTE].asString())


class passwordmanager : public QMainWindow
{
	Q_OBJECT

public:
	passwordmanager(QWidget* parent = nullptr);

	~passwordmanager();

public:
	Json::Value root;
	QString password;
	char* path;
	QNetworkAccessManager* manager;
private:
	Ui::passwordmanagerClass ui;

private slots:
	void add_entry();
	void sync_entry();
	void push_entry();
	void handle_item_double_clicked(QTreeWidgetItem* item);

private:
	void edit_entry(QTreeWidgetItem* item, QString& category, QString& url, QString& login_name, QString& password, QString& note);
	void refresh_tree_item();
	void show_context_menu(const QPoint& pos);
	void delete_item(QTreeWidgetItem* item);
	void edit_item(QTreeWidgetItem* item);
	void add_json(QString& category, QString& url, QString& login_name, QString& password, QString& remark);
	void edit_json(QString& old_category, QString& old_url, QString& old_login_name, QString& old_password, QString& old_remark,
		QString& new_category, QString& new_url, QString& new_login_name, QString& new_password, QString& new_remark);
public:
	int init_data(Json::Value& data);
	void write_to_file();
};
