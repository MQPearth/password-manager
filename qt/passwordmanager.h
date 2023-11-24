#pragma once

#include "ui_passwordmanager.h"
#include <jsoncpp/json.h>


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
    passwordmanager(QWidget *parent = nullptr);

    ~passwordmanager();

public:
    Json::Value root;
    QString password;
    char* path;

private:
    Ui::passwordmanagerClass ui;

private slots:
    void add_entry();

private:
    void refresh_tree_item();
    void showContextMenu(const QPoint& pos);
    void deleteItem(QTreeWidgetItem* item);
    void editItem(QTreeWidgetItem* item);
    void add_json(QString &category, QString& url, QString& login_name, QString& password, QString& remark);
public:
    int init_data(QString& txt);
};
