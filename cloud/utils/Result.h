//
// Created by zzx on 2023/11/12.
//

#ifndef CLOUD_RESULT_H
#define CLOUD_RESULT_H

#include "drogon/drogon.h"
#include "drogon/HttpResponse.h"


class Result {
public:
    static drogon::HttpResponsePtr ok();

    static Json::Value json();

    static drogon::HttpResponsePtr error();

    static drogon::HttpResponsePtr error(const std::string &msg);

};


#endif //CLOUD_RESULT_H
