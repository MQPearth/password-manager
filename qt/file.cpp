#include "file.h"
#include "encrypt.h"
#include <filesystem>
#include <QTextStream>
#include <QObject>
#include <QFile>
#include <QDir>

QString file_util::get_bin_file_path(const char* path) {
    QString fullPath = QString::fromUtf8(path);
    QFileInfo fileInfo(fullPath);

    return fileInfo.absolutePath() + "/" + BIN_FILE;
}


QString file_util::get_server_file_path(const char* path) {
    QString fullPath = QString::fromUtf8(path);
    QFileInfo fileInfo(fullPath);

    return fileInfo.absolutePath() + "/" + SERVER_FILE;
}


bool file_util::check_bin_file(const char* path)
{
    return QFile(get_bin_file_path(path)).exists();
}

void file_util::write(const char* path, const QString& txt, QString& pwd)
{
    auto content = encrypt_util::encrypt(txt, pwd);
    QFile file(get_bin_file_path(path));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << content;
        file.close();
    }
}

QString file_util::read(const char* path, QString& pwd)
{
    QFile file(get_bin_file_path(path));
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        content = in.readAll();
        file.close();
    }

    return encrypt_util::decrypt(content, pwd);
}