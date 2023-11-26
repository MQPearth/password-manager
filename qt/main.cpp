#include "passwordmanager.h"
#include <QtWidgets/QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
#include <QStyleFactory>
#include "encrypt.h"
#include "file.h"




QString get_password(bool exist, passwordmanager *w) {

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

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);
    passwordmanager w;

    w.setWindowTitle("密码管理器");
    w.show();

    w.path = argv[0];

    bool exist = file_util::check_bin_file(w.path);

    QString password = get_password(exist, &w);
    qDebug() << password;
    if (password.isEmpty()) {
        return -1;
    }

    w.password = password;
    
    QString file_content;

    if (!exist) {
        QMessageBox* msgBox = new QMessageBox(&w);
        msgBox->setText("<p>&nbsp;&nbsp;&nbsp;&nbsp;请牢记你的密码&nbsp;&nbsp;&nbsp;&nbsp;</p> <p>&nbsp;&nbsp;&nbsp;&nbsp;[" + 
            password + "]&nbsp;&nbsp;&nbsp;&nbsp;</p><p style='color: red'>&nbsp;&nbsp;&nbsp;&nbsp;密码丢失即数据丢失&nbsp;&nbsp;&nbsp;&nbsp;</p>");
        msgBox->exec();
        delete msgBox;
        file_content = R"({"verify": true, "data": []})";
    }
    else {
        try {
            file_content = file_util::read(w.path, password);
        }
        catch (...) {
            QMessageBox* msgBox = new QMessageBox(&w);
            msgBox->setText("解密文件失败, 请检查密码");
            msgBox->exec();
            return -1;
        }
    }

    int code = w.init_data(file_content);

    if (code) {
        QMessageBox* msgBox = new QMessageBox(&w);
        msgBox->setText("加密文件内容错误: " + file_content);
        msgBox->exec();
        return -1;
    }

    if (w.root["data"].size() == 0) {
        w.write_to_file();
    }
    
    return a.exec();
}
