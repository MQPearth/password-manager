#include "passwordmanager.h"

#include <QLineEdit>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <jsoncpp/json.h>

passwordmanager::passwordmanager(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionAdd, &QAction::triggered, this, &passwordmanager::add_entry);

	ui.treeWidget->setHeaderLabels(QStringList() << "分类" << "URL" << "登录名" << "密码" << "备注");
}

void passwordmanager::init_data(QString& txt) {

	Json::CharReaderBuilder reader;

	std::istringstream jsonStream(txt.toStdString());

	Json::parseFromStream(reader, jsonStream, &this->root, nullptr);
}

bool compareEntries(const Json::Value& l, const Json::Value& r) {
	auto category_l = l["category"].asString();
	auto category_r = r["category"].asString();
	if (category_l != category_r)
		return category_l < category_r;

	auto url_l = l["url"].asString();
	auto url_r = r["url"].asString();
	if (url_l != url_r)
		return url_l < url_r;

	auto login_name_l = l["login_name"].asString();
	auto login_name_r = r["login_name"].asString();
	if (login_name_l != login_name_r)
		return login_name_l < login_name_r;

	return l["password"].asString() < r["password"].asString();
}

void passwordmanager::refresh_tree_item() {

	Json::Value data = this->root["data"];

	std::vector<Json::Value> json_vector;

	for (const auto& entry : data) {
		json_vector.push_back(entry);
	}

	std::sort(json_vector.begin(), json_vector.end(), compareEntries);


	ui.treeWidget->clear();
	Json::Value* last_json_ele = nullptr;
	QTreeWidgetItem* last_item_ele = nullptr;

	for (auto& entry : json_vector) {
		qDebug() << entry.toStyledString().c_str();

		int step = 0;
		qDebug() << last_json_ele;
		if (last_json_ele && GET_CATEGORY(entry) == GET_CATEGORY((*last_json_ele))) {
			step++;
			if (last_json_ele && GET_URL(entry) == GET_URL((*last_json_ele))) {
				step++;
				if (last_json_ele && GET_LOGIN_NAME(entry) == GET_LOGIN_NAME((*last_json_ele))) {
					step++;
					QTreeWidgetItem* password = new QTreeWidgetItem(*last_item_ele);
					password->setText(step++, GET_PASSWORD_Q(entry));
					password->setText(step++, GET_NOTE_Q(entry));
					last_item_ele->addChild(password);
					last_item_ele = password;
				}
				else {
					QTreeWidgetItem* login_name = new QTreeWidgetItem(*last_item_ele);
					login_name->setText(step++, GET_LOGIN_NAME_Q(entry));
					login_name->setText(step++, GET_PASSWORD_Q(entry));
					login_name->setText(step++, GET_NOTE_Q(entry));
					last_item_ele->addChild(login_name);
					last_item_ele = login_name;
				}
			}
			else {
				QTreeWidgetItem* url = new QTreeWidgetItem(*last_item_ele);
				url->setText(step++, GET_URL_Q(entry));
				url->setText(step++, GET_LOGIN_NAME_Q(entry));
				url->setText(step++, GET_PASSWORD_Q(entry));
				url->setText(step++, GET_NOTE_Q(entry));
				last_item_ele->addChild(url);
				last_item_ele = url;
			}
		}
		else {
			QTreeWidgetItem* category = new QTreeWidgetItem(ui.treeWidget);
			category->setText(step++, GET_CATEGORY_Q(entry));
			category->setText(step++, GET_URL_Q(entry));
			category->setText(step++, GET_LOGIN_NAME_Q(entry));
			category->setText(step++, GET_PASSWORD_Q(entry));
			category->setText(step++, GET_NOTE_Q(entry));
			ui.treeWidget->addTopLevelItem(category);
			last_item_ele = category;
		}
		last_json_ele = &entry;
	}

	if (!last_json_ele) {
		delete last_json_ele;
	}


	/*
	for (int i = 0; i < data.size(); i++) {
		Json::Value ele = data[i];

		QTreeWidgetItem* category = new QTreeWidgetItem(ui.treeWidget);
		category->setText(0, QString::fromStdString(ele["category"].asString()));
		Json::Value oneSubEle = ele["sub"];
		for (int j = 0; j < oneSubEle.size(); j++) {
			Json::Value twoSubEle = oneSubEle[i];

			QTreeWidgetItem* url = new QTreeWidgetItem(category);
			url->setText(1, QString::fromStdString(twoSubEle["url"].asString()));

			for (int k = 0; k < twoSubEle.size(); k++) {
				Json::Value subEle = twoSubEle[i];

				QTreeWidgetItem* item = new QTreeWidgetItem(url);
				item->setText(2, QString::fromStdString(subEle["login_name"].asString()));
				item->setText(3, QString::fromStdString(subEle["password"].asString()));
				item->setText(4, QString::fromStdString(subEle["remark"].asString()));

				url->addChild(item);
			}

			category->addChild(url);
		}


		ui.treeWidget->addTopLevelItem(category);
	}
	*/
}

/* json format
  {
	"verify": true,
	"data": [
		{
			"category": "qq",
			"url": "https://qq.com",
			"login_name": "test",
			"password": "123123",
			"note": ""
		}
	]
  }
 */
void passwordmanager::add_json(QString& category, QString& url, QString& login_name, QString& password, QString& note) {

	Json::Value ele;

	ele["category"] = category.toStdString();
	ele["url"] = url.toStdString();
	ele["login_name"] = login_name.toStdString();
	ele["password"] = password.toStdString();
	ele["note"] = note.toStdString();

	root["data"].append(ele);

	this->refresh_tree_item();
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

passwordmanager::~passwordmanager()
{}
