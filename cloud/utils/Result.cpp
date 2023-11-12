//
// Created by zzx on 2023/11/12.
//

#include "Result.h"
#include "drogon/HttpResponse.h"

using namespace drogon;

drogon::HttpResponsePtr Result::ok() {
    Json::Value ret;
    ret["code"] = "0";
    ret["msg"] = "ok";
    ret["data"] = Json::nullValue;
    return HttpResponse::newHttpJsonResponse(ret);
}

Json::Value Result::json() {
    Json::Value ret;
    ret["code"] = "0";
    ret["msg"] = "ok";
    return ret;
}

drogon::HttpResponsePtr Result::error() {
    Json::Value ret;
    ret["code"] = "99";
    ret["msg"] = "error";
    return HttpResponse::newHttpJsonResponse(ret);
}

drogon::HttpResponsePtr Result::error(const std::string &msg) {
    Json::Value ret;
    ret["code"] = "99";
    ret["msg"] = msg;
    return HttpResponse::newHttpJsonResponse(ret);
}