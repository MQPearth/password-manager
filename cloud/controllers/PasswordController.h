#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class PasswordController : public drogon::HttpController<PasswordController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(PasswordController::get, "/pwd/get", Post);
        ADD_METHOD_TO(PasswordController::update, "/pwd/update", Put);
    METHOD_LIST_END

    /**
     * get
     * @param req
     * @param callback
     */
    static void get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

    /**
    * update
    * @param req
    * @param callback
    */
    static void update(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};
