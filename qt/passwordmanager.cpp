#include "passwordmanager.h"
#include "tree_item.h"
#include "file.h"
#include "json.h"
#include "cloud.h"
#include "encrypt.h"

#include <QLineEdit>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QApplication>
#include <QScreen>
#include <QTreeWidget>
#include <QMessageBox>
#include <QClipboard>

#include <jsoncpp/json.h>
#include "cloud.h"
#include "message.h"



passwordmanager::passwordmanager(QWidget* parent) : QMainWindow(parent), path(nullptr), password(QString())
{
	ui.setupUi(this);

	connect(ui.actionAdd, &QAction::triggered, this, &passwordmanager::add_entry);

	connect(ui.actionSync, &QAction::triggered, this, &passwordmanager::sync_entry);

	connect(ui.actionPush, &QAction::triggered, this, &passwordmanager::push_entry);

	ui.treeWidget->setHeaderLabels(QStringList() << "分类" << "URL" << "登录名" << "密码" << "备注");

	ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeWidget, &QTreeWidget::customContextMenuRequested, this, &passwordmanager::show_context_menu);

	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &passwordmanager::handle_item_double_clicked);

	QHeaderView* header = ui.treeWidget->header();

	header->resizeSection(0, 80);
	header->resizeSection(1, 160);
	header->resizeSection(2, 120);
	header->resizeSection(3, 140);
	header->resizeSection(4, 170);
	QNetworkAccessManager* manager = new QNetworkAccessManager(this);
	this->manager = manager;
}

void passwordmanager::delete_item(QTreeWidgetItem* item) {

	QMessageBox::StandardButton reply;
	reply = QMessageBox::critical(this, "删除", "确定删除?",
		QMessageBox::Yes | QMessageBox::No);
	if (!(reply == QMessageBox::Yes)) {
		return;
	}

	std::string category;

	QTreeWidgetItem* category_item = item;

	do {
		category = category_item->text(0).toStdString();
	} while (category.empty() && (category_item = category_item->parent()));

	std::string url;

	QTreeWidgetItem* url_item = item;

	do {
		url = url_item->text(1).toStdString();
	} while (url.empty() && (url_item = url_item->parent()));

	std::string login_name;

	QTreeWidgetItem* login_name_item = item;

	do {
		login_name = login_name_item->text(2).toStdString();
	} while (login_name.empty() && (login_name_item = login_name_item->parent()));

	std::string password;

	QTreeWidgetItem* password_item = item;

	do {
		password = password_item->text(3).toStdString();
	} while (password.empty() && (password_item = password_item->parent()));

	auto note = item->text(4);

	QTreeWidgetItem* parent = item->parent();
	if (parent) {
		delete parent->takeChild(parent->indexOfChild(item));
	}
	else {
		delete ui.treeWidget->takeTopLevelItem(ui.treeWidget->indexOfTopLevelItem(item));
	}

	auto data = this->root["data"];
	for (Json::Value::ArrayIndex i = 0; i < data.size(); ++i) {

		Json::Value& item = data[i];

		qDebug() << item.toStyledString().c_str();

		if (GET_CATEGORY(item) == category && GET_URL(item) == url && GET_LOGIN_NAME(item) == login_name
			&& GET_PASSWORD(item) == password && GET_NOTE(item) == note.toStdString()) {
			data.removeIndex(i, &item);
			this->root["data"] = data;
			this->write_to_file();
			break;
		}
	}
}

void passwordmanager::edit_item(QTreeWidgetItem* item) {

	QString category;

	QTreeWidgetItem* category_item = item;

	do {
		category = category_item->text(0);
	} while (category.isEmpty() && (category_item = category_item->parent()));

	QString url;

	QTreeWidgetItem* url_item = item;

	do {
		url = url_item->text(1);
	} while (url.isEmpty() && (url_item = url_item->parent()));

	QString login_name;

	QTreeWidgetItem* login_name_item = item;

	do {
		login_name = login_name_item->text(2);
	} while (login_name.isEmpty() && (login_name_item = login_name_item->parent()));

	QString password;

	QTreeWidgetItem* password_item = item;

	do {
		password = password_item->text(3);
	} while (password.isEmpty() && (password_item = password_item->parent()));


	QString note;

	QTreeWidgetItem* note_item = item;

	do {
		note = note_item->text(4);
	} while (note.isEmpty() && (note_item = note_item->parent()));

	
	this->edit_entry(item, category, url, login_name, password, note);
}


void passwordmanager::show_context_menu(const QPoint& pos) {
	QTreeWidgetItem* item = ui.treeWidget->itemAt(pos);

	if (item != nullptr) {
		QMenu* menu = new QMenu(this);

		QAction* deleteAction = new QAction("删除", this);
		connect(deleteAction, &QAction::triggered, [this, item]() {
			delete_item(item);
			});
		menu->addAction(deleteAction);

		QAction* editAction = new QAction("修改", this);
		connect(editAction, &QAction::triggered, [this, item]() {
			edit_item(item);
			});
		menu->addAction(editAction);

		menu->exec(ui.treeWidget->mapToGlobal(pos));
	}
}



int passwordmanager::init_data(Json::Value& data) {
	try {
		this->root = data;

		if (!(this->root["verify"].asBool())) {
			return -1;
		}

		this->refresh_tree_item();
		return 0;
	}
	catch (...) {
		return -1;
	}
}

bool compare_entries(const Json::Value& l, const Json::Value& r) {
	auto category_l = GET_CATEGORY(l);
	auto category_r = GET_CATEGORY(r);
	if (category_l != category_r)
		return category_l < category_r;

	auto url_l = GET_URL(l);
	auto url_r = GET_URL(r);
	if (url_l != url_r)
		return url_l < url_r;

	auto login_name_l = GET_LOGIN_NAME(l);
	auto login_name_r = GET_LOGIN_NAME(r);
	if (login_name_l != login_name_r)
		return login_name_l < login_name_r;

	return GET_PASSWORD(l) < GET_PASSWORD(r);
}

void passwordmanager::refresh_tree_item() {

	Json::Value data = this->root["data"];

	std::vector<Json::Value> json_vector;

	for (const auto& entry : data) {
		json_vector.push_back(entry);
	}

	std::sort(json_vector.begin(), json_vector.end(), compare_entries);

	ui.treeWidget->clear();

	// build tree
	std::map<std::string, QTreeWidgetItem*> category_map;
	std::map<tree_item_url, QTreeWidgetItem*> category_url_map;
	std::map<tree_item_login_name, QTreeWidgetItem*> category_url_login_name_map;

	for (const auto& item : json_vector) {
		qDebug() << item.toStyledString().c_str();
		auto category_ele = category_map[GET_CATEGORY(item)];
		if (category_ele) {
			auto category_url_ele = category_url_map[tree_item_url(GET_CATEGORY(item), GET_URL(item))];
			if (category_url_ele) {
				auto category_url_login_name_ele = category_url_login_name_map
					[tree_item_login_name(GET_CATEGORY(item), GET_URL(item), GET_LOGIN_NAME(item))];
				if (category_url_login_name_ele) {
					QTreeWidgetItem* password = new QTreeWidgetItem(category_url_login_name_ele);
					password->setText(3, GET_PASSWORD_Q(item));
					password->setText(4, GET_NOTE_Q(item));
				}
				else {
					QTreeWidgetItem* login_name = new QTreeWidgetItem(category_url_ele);
					login_name->setText(2, GET_LOGIN_NAME_Q(item));
					login_name->setText(3, GET_PASSWORD_Q(item));
					login_name->setText(4, GET_NOTE_Q(item));

					category_url_login_name_map[tree_item_login_name(GET_CATEGORY(item), GET_URL(item), GET_LOGIN_NAME(item))] = login_name;
				}
			}
			else {
				QTreeWidgetItem* url = new QTreeWidgetItem(category_ele);
				url->setText(1, GET_URL_Q(item));
				url->setText(2, GET_LOGIN_NAME_Q(item));
				url->setText(3, GET_PASSWORD_Q(item));
				url->setText(4, GET_NOTE_Q(item));

				category_url_map[tree_item_url(GET_CATEGORY(item), GET_URL(item))] = url;
				category_url_login_name_map[tree_item_login_name(GET_CATEGORY(item), GET_URL(item), GET_LOGIN_NAME(item))] = url;
			}
		}
		else {
			QTreeWidgetItem* category = new QTreeWidgetItem(ui.treeWidget);
			category->setText(0, GET_CATEGORY_Q(item));
			category->setText(1, GET_URL_Q(item));
			category->setText(2, GET_LOGIN_NAME_Q(item));
			category->setText(3, GET_PASSWORD_Q(item));
			category->setText(4, GET_NOTE_Q(item));

			category_map[GET_CATEGORY(item)] = category;
			category_url_map[tree_item_url(GET_CATEGORY(item), GET_URL(item))] = category;
			category_url_login_name_map[tree_item_login_name(GET_CATEGORY(item), GET_URL(item), GET_LOGIN_NAME(item))] = category;
		}
	}

}


void passwordmanager::handle_item_double_clicked(QTreeWidgetItem* item)
{
	qDebug() << "category: " << item->text(0) << "url: " << item->text(1) << "login_name: " << item->text(2)
		<< "password: " << item->text(3) << "note: " << item->text(4);

	QClipboard* clipboard = QApplication::clipboard();

	std::string url;

	QTreeWidgetItem* url_item = item;

	do {
		url = url_item->text(1).toStdString();
	} while (url.empty() && (url_item = url_item->parent()));

	std::string login_name;

	QTreeWidgetItem* login_name_item = item;

	do {
		login_name = login_name_item->text(2).toStdString();
	} while (login_name.empty() && (login_name_item = login_name_item->parent()));

	std::string password;

	QTreeWidgetItem* password_item = item;

	do {
		password = password_item->text(3).toStdString();
	} while (password.empty() && (password_item = password_item->parent()));

	std::stringstream ss;
	ss << "网址: " << url << "\n" << "登录名: " << login_name << "\n" << "密码: " << password;

	clipboard->setText(QString::fromStdString(ss.str()));

	QMessageBox* msgBox = new QMessageBox(this);
	msgBox->setText("已复制到剪切板");
	msgBox->exec();
}

void passwordmanager::add_json(QString& category, QString& url, QString& login_name, QString& password, QString& note) {

	Json::Value ele;

	ele["category"] = category.toStdString();
	ele["url"] = url.toStdString();
	ele["login_name"] = login_name.toStdString();
	ele["password"] = password.toStdString();
	ele["note"] = note.toStdString();

	root["data"].append(ele);

	this->write_to_file();

	this->refresh_tree_item();
}

void passwordmanager::push_entry() {
	auto enc_data = encrypt_util::encrypt(QString::fromUtf8(this->root.toStyledString().c_str()), this->password);
	auto server_config_txt = file_util::read_server(this->path, this->password);

	auto server_config = json_util::parse(server_config_txt);

	auto cloud_host = QString::fromStdString(server_config["host"].asString());
	auto cloud_login_name = QString::fromStdString(server_config["login_name"].asString());
	auto cloud_password = QString::fromStdString(server_config["password"].asString());

	auto resp = cloud_util::update(this, cloud_host, cloud_login_name, cloud_password, enc_data);
	if (resp["code"] == "0") {
		message_util::show(this, "备份完成");
	}
	else {
		auto resp_msg = resp["msg"].asString();
		auto msg = QString::fromStdString("备份失败: " + resp_msg.empty() ? "网络错误": resp_msg);
		message_util::show(this, msg);
	}
}

void passwordmanager::sync_entry() {
	bool exist = file_util::check_server_file(this->path);

	QDialog dialog(this);
	dialog.setWindowTitle("云同步管理");
	dialog.setMinimumWidth(400);

	QFormLayout formLayout(&dialog);

	QLineEdit* hostEdit = new QLineEdit(&dialog);
	hostEdit->setPlaceholderText("请填写Cloud服务Host");

	QLineEdit* loginNameEdit = new QLineEdit(&dialog);
	loginNameEdit->setPlaceholderText("请填写用户名");

	QLineEdit* passwordEdit = new QLineEdit(&dialog);
	passwordEdit->setPlaceholderText("请填写密码");


	formLayout.addRow("<p>Host: <span style='color: red'>*</span></p>", hostEdit);
	formLayout.addRow("<p>用户名:  <span style='color: red'>*</span></p>", loginNameEdit);
	formLayout.addRow("<p>密码: <span style='color: red'>*</span></p>", passwordEdit);

	if (exist) {
		QString server_config_txt = file_util::read_server(this->path, this->password);
		Json::Value server_config = json_util::parse(server_config_txt);

		hostEdit->setText(QString::fromStdString(server_config["host"].asString()));
		loginNameEdit->setText(QString::fromStdString(server_config["login_name"].asString()));
		passwordEdit->setText(QString::fromStdString(server_config["password"].asString()));
	}


	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	formLayout.addRow(&buttonBox);

	connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	while (1 && dialog.exec() == QDialog::Accepted) {

		QString host = hostEdit->text();
		QString login_name = loginNameEdit->text();
		QString password = passwordEdit->text();
		
		if (host.isEmpty()) {
			hostEdit->setPlaceholderText("host是必填项");
			continue;
		}
		if (login_name.isEmpty()) {
			loginNameEdit->setPlaceholderText("用户名是必填项");
			continue;
		}
		if (password.isEmpty()) {
			passwordEdit->setPlaceholderText("密码是必填项");
			continue;
		}

		Json::Value new_config;
		new_config["host"] = host.toStdString();
		new_config["login_name"] = login_name.toStdString();
		new_config["password"] = password.toStdString();

		file_util::write_server(this->path, QString::fromStdString(new_config.toStyledString()), this->password);

		break;
	}
}

void passwordmanager::add_entry()
{
	// 创建对话框
	QDialog dialog(this);
	dialog.setWindowTitle("新增密码");
	dialog.setMinimumWidth(400);

	// 创建表单布局
	QFormLayout formLayout(&dialog);

	QLineEdit* categoryEdit = new QLineEdit(&dialog);
	categoryEdit->setPlaceholderText("请填写分类");

	QLineEdit* urlEdit = new QLineEdit(&dialog);
	urlEdit->setPlaceholderText("请填写url");

	QLineEdit* usernameEdit = new QLineEdit(&dialog);
	usernameEdit->setPlaceholderText("请填写用户名");

	QLineEdit* passwordEdit = new QLineEdit(&dialog);
	passwordEdit->setPlaceholderText("请填写密码");

	QLineEdit* noteEdit = new QLineEdit(&dialog);
	noteEdit->setPlaceholderText("请填写备注");

	formLayout.addRow("<p>分类: <span style='color: red'>*</span></p>", categoryEdit);
	formLayout.addRow("<p>URL:  <span style='color: red'>*</span></p>", urlEdit);
	formLayout.addRow("<p>登录名: <span style='color: red'>*</span></p>", usernameEdit);
	formLayout.addRow("<p>密码: <span style='color: red'>*</span></p>", passwordEdit);
	formLayout.addRow("备注:", noteEdit);


	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	formLayout.addRow(&buttonBox);

	connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	while (1 && dialog.exec() == QDialog::Accepted) {

		QString category = categoryEdit->text();
		QString url = urlEdit->text();
		QString username = usernameEdit->text();
		QString password = passwordEdit->text();
		QString note = noteEdit->text();
		if (category.isEmpty()) {
			categoryEdit->setPlaceholderText("分类是必填项");
			continue;
		}
		if (url.isEmpty()) {
			urlEdit->setPlaceholderText("url是必填项");
			continue;
		}
		if (username.isEmpty()) {
			usernameEdit->setPlaceholderText("用户名是必填项");
			continue;
		}
		if (password.isEmpty()) {
			passwordEdit->setPlaceholderText("密码是必填项");
			continue;
		}

		this->add_json(category, url, username, password, note);
		break;
	}

}


void passwordmanager::edit_entry(QTreeWidgetItem* item, QString& category, QString& url, QString& login_name, 
	QString& password, QString& note)
{
	QDialog dialog(this);
	dialog.setWindowTitle("修改密码");
	dialog.setMinimumWidth(400);

	QFormLayout formLayout(&dialog);

	QLineEdit* categoryEdit = new QLineEdit(&dialog);
	categoryEdit->setPlaceholderText("请填写分类");
	categoryEdit->setText(category);

	QLineEdit* urlEdit = new QLineEdit(&dialog);
	urlEdit->setPlaceholderText("请填写url");
	urlEdit->setText(url);

	QLineEdit* usernameEdit = new QLineEdit(&dialog);
	usernameEdit->setPlaceholderText("请填写用户名");
	usernameEdit->setText(login_name);

	QLineEdit* passwordEdit = new QLineEdit(&dialog);
	passwordEdit->setPlaceholderText("请填写密码");
	passwordEdit->setText(password);

	QLineEdit* noteEdit = new QLineEdit(&dialog);
	noteEdit->setPlaceholderText("请填写备注");
	noteEdit->setText(note);
	

	formLayout.addRow("<p>分类: <span style='color: red'>*</span></p>", categoryEdit);
	formLayout.addRow("<p>URL:  <span style='color: red'>*</span></p>", urlEdit);
	formLayout.addRow("<p>登录名: <span style='color: red'>*</span></p>", usernameEdit);
	formLayout.addRow("<p>密码: <span style='color: red'>*</span></p>", passwordEdit);
	formLayout.addRow("备注:", noteEdit);


	QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, &dialog);
	formLayout.addRow(&buttonBox);

	connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	while (1 && dialog.exec() == QDialog::Accepted) {

		QString new_category = categoryEdit->text();
		QString new_url = urlEdit->text();
		QString new_username = usernameEdit->text();
		QString new_password = passwordEdit->text();
		QString new_note = noteEdit->text();
		if (new_category.isEmpty()) {
			categoryEdit->setPlaceholderText("分类是必填项");
			continue;
		}
		if (new_url.isEmpty()) {
			urlEdit->setPlaceholderText("url是必填项");
			continue;
		}
		if (new_username.isEmpty()) {
			usernameEdit->setPlaceholderText("用户名是必填项");
			continue;
		}
		if (new_password.isEmpty()) {
			passwordEdit->setPlaceholderText("密码是必填项");
			continue;
		}

		this->edit_json(category, url, login_name, password, note, new_category, new_url, new_username, new_password, new_note);

		this->refresh_tree_item();
		break;
	}

}

void passwordmanager::edit_json(QString& old_category, QString& old_url, QString& old_login_name, QString& old_password, QString& old_remark,
	QString& new_category, QString& new_url, QString& new_login_name, QString& new_password, QString& new_remark) {

	auto data = this->root["data"];
	for (Json::Value::ArrayIndex i = 0; i < data.size(); ++i) {

		Json::Value item = data[i];

		qDebug() << item.toStyledString().c_str();

		if (GET_CATEGORY(item) == old_category.toStdString() && GET_URL(item) == old_url.toStdString()
			&& GET_LOGIN_NAME(item) == old_login_name.toStdString() && GET_PASSWORD(item) == old_password.toStdString()
			&& GET_NOTE(item) == old_remark.toStdString()) {

			Json::Value new_item;
			new_item[CATEGORY] = new_category.toStdString();
			new_item[URL] = new_url.toStdString();
			new_item[LOGIN_NAME] = new_login_name.toStdString();
			new_item[PASSWORD] = new_password.toStdString();
			new_item[NOTE] = new_remark.toStdString();

			data[i] = new_item;

			this->root["data"] = data;

			this->write_to_file();
			break;
		}
	}

}


void passwordmanager::write_to_file() {
	file_util::write(this->path, QString::fromUtf8(this->root.toStyledString().c_str()), this->password);
}

passwordmanager::~passwordmanager()
{}