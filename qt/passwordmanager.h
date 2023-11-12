#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_passwordmanager.h"

class passwordmanager : public QMainWindow
{
    Q_OBJECT

public:
    passwordmanager(QWidget *parent = nullptr);
    ~passwordmanager();

private:
    Ui::passwordmanagerClass ui;
};
