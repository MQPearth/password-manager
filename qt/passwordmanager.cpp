#include "passwordmanager.h"

#include <QLineEdit>
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


void passwordmanager::refresh_tree_item() {

	Json::Value data = root["data"];

	








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
