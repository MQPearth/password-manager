#include "passwordmanager.h"
#include <QtWidgets/QApplication>
#include <QInputDialog>
#include <iostream>
#include <QDebug>
#include <filesystem>
#include "encrypt_util.h"


#define BIN_FILE "enc_data.bin";
#define SERVER_FILE "server.bin";


bool check_bin_file(char* path) {
    namespace fs = std::filesystem;

    fs::path executablePath = fs::canonical(fs::path(path));

    fs::path specifiedFilePath = executablePath.parent_path() / BIN_FILE;

    return fs::exists(specifiedFilePath);
}


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

    bool exist = check_bin_file(argv[0]);

    QString password = get_password(exist, &w);
    qDebug() << password;
    if (password.isEmpty()) {
        return -1;
    }

    QString enc = encrypt_util::encrypt("test", "test");
    qDebug() << enc;
    qDebug() << encrypt_util::decrypt(enc, "test");
    
    return a.exec();
}
