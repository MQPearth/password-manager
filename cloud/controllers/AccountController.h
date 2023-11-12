#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class AccountController : public drogon::HttpController<AccountController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(AccountController::login, "/account/login", Post);
    METHOD_LIST_END

    /**
     * login
     * @param req
     * @param callback
     */
    static void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};
