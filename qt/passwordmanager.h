#pragma once

#include "ui_passwordmanager.h"
#include <jsoncpp/json.h>


class passwordmanager : public QMainWindow
{
    Q_OBJECT

public:
    passwordmanager(QWidget *parent = nullptr);
    ~passwordmanager();

public:
    Json::Value root;

private:
    Ui::passwordmanagerClass ui;

private slots:
    void add_entry();

private:
    void refresh_tree_item();
    void add_json(QString &category, QString& url, QString& login_name, QString& password, QString& remark);
public:
    void init_data(QString& txt);
};
