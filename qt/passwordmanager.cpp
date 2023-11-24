#include "passwordmanager.h"
#include "tree_item.h"

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

	QHeaderView* header = ui.treeWidget->header();

	header->setSectionResizeMode(0, QHeaderView::Fixed);
	header->resizeSection(0, 80);

	header->setSectionResizeMode(1, QHeaderView::Fixed);
	header->resizeSection(1, 160);

	header->setSectionResizeMode(2, QHeaderView::Fixed);
	header->resizeSection(2, 120);

	header->setSectionResizeMode(3, QHeaderView::Fixed);
	header->resizeSection(3, 140);

	header->setSectionResizeMode(4, QHeaderView::Fixed);
	header->resizeSection(4, 170);
}

void passwordmanager::init_data(QString& txt) {

	Json::CharReaderBuilder reader;

	std::istringstream jsonStream(txt.toStdString());

	Json::parseFromStream(reader, jsonStream, &this->root, nullptr);

	this->refresh_tree_item();
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
