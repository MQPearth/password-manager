#include "AccountController.h"
#include "Account.h"
#include "utils/Result.h"


// Add definition of your processing function here
void AccountController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

    auto json = req->getJsonObject();
    drogon_model::password::Account account;
    if (!json) {
        callback(Result::error());
        return;
    }

    auto login_name = (*json)["login_name"];
    if (login_name.empty() || login_name.asString().empty()) {
        callback(Result::error("login_name不能为空"));
        return;
    }
    auto password = (*json)["password"];
    if (password.empty() || password.asString().empty()) {
        callback(Result::error("password不能为空"));
        return;
    }



    auto ret = Result::json();
    ret["data"] = "token";
    callback(HttpResponse::newHttpJsonResponse(ret));
}