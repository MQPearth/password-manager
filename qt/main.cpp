#include "passwordmanager.h"
#include <QtWidgets/QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QDebug>
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
    
    QString file_content = file_util::read(w.path, password);

    int code = w.init_data(file_content);

    if (code) {
        QMessageBox msgBox;
        msgBox.setText("加密文件内容错误");
        msgBox.exec();
        return -1;
    }

    QString enc = encrypt_util::encrypt("test", "test");
    qDebug() << enc;
    qDebug() << encrypt_util::decrypt(enc, "test");
    
    return a.exec();
}
