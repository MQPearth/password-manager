#include "passwordmanager.h"

#include <QLineEdit>
#include <QDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <jsoncpp/json.h>

passwordmanager::passwordmanager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.actionAdd, &QAction::triggered, this, &passwordmanager::add_entry);

    ui.treeWidget->setHeaderLabels(QStringList() << "分类" << "URL" << "登录名" << "密码" << "备注");
  
}

void passwordmanager::add_json(QString& category, QString& url, QString& login_name, QString& password, QString& remark) {
    Json::Value data = root["data"];

    for (int i = 0; i < data.size(); i++) {
        Json::Value ele = data[i];
        if (category == QString::fromStdString(ele["category"].asString())) {
            
        }
    }
}


void passwordmanager::refresh_tree_item() {

    Json::Value data = root["data"];

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
    QLineEdit* urlEdit = new QLineEdit(&dialog);
    QLineEdit* usernameEdit = new QLineEdit(&dialog);
    QLineEdit* passwordEdit = new QLineEdit(&dialog);
    QLineEdit* noteEdit = new QLineEdit(&dialog);

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


    if (dialog.exec() == QDialog::Accepted) {

        QString category = categoryEdit->text();
        QString url = urlEdit->text();
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        QString note = noteEdit->text();

        // 在这里可以使用输入的值进行其他操作
    }
}

passwordmanager::~passwordmanager()
{}
