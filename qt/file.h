#pragma once

#include <QString>

#define BIN_FILE "enc_data.bin";
#define SERVER_FILE "server.bin";

class file_util
{
public:
    static bool check_bin_file(const char* path);

    static bool check_server_file(const char* path);

    static void write_server(const char* path, const QString& txt, QString& pwd);

    static void write(const char* path, const QString& txt, QString& pwd);

    static QString read_server(const char* path, QString &pwd);

    static QString read(const char* path, QString &pwd);

    static QString get_bin_file_path(const char* path);

    static QString get_server_file_path(const char* path);

    static long long get_bin_file_last_modify_time(const char* path);
};

