#include "PasswordController.h"
#include "Account.h"
#include "Password.h"
#include "utils/Result.h"
#include <drogon/orm/Mapper.h>

using namespace drogon_model::password;

using namespace drogon;

void PasswordController::get(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

    auto json = req->getJsonObject();
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
    auto client = app().getDbClient();
    orm::Mapper<Account> account_mapper(client);

    try {
        auto row = account_mapper.findOne(orm::Criteria(Account::Cols::_login_name,
                                                        orm::CompareOperator::EQ, login_name));

        if (*(row.getPassword()) != password.asString()) {
            callback(Result::error("账户名或密码错误"));
            return;
        }

        row.setLatestLoginDate(trantor::Date::now());
        account_mapper.update(row);

        orm::Mapper<Password> password_mapper(client);

        auto enc_data_row = password_mapper.findOne(orm::Criteria(Password::Cols::_id,
                                                                  orm::CompareOperator::EQ, *row.getId()));

        auto ret = Result::json();
        const auto &enc_data = enc_data_row.getEncData();
        Json::Value data;
        data["enc_data"] = *enc_data;
        data["update_time"] = static_cast<Json::Int64>(enc_data_row.getUpdateTime()->secondsSinceEpoch());

        ret["data"] = data;
        callback(HttpResponse::newHttpJsonResponse(ret));
    } catch (orm::UnexpectedRows &e) {
        callback(Result::error("账户名或密码错误"));
    }
    catch (...) {
        callback(Result::error());
    }
}


void PasswordController::update(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
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

    auto enc_data = (*json)["enc_data"];
    if (enc_data.empty()) {
        callback(Result::error("enc_data不能为null"));
        return;
    }

    auto client = app().getDbClient()->newTransaction();

    try {

        orm::Mapper<Account> account_mapper(client);

        auto row = account_mapper.findOne(orm::Criteria(Account::Cols::_login_name,
                                                        orm::CompareOperator::EQ, login_name));

        if (*(row.getPassword()) != password.asString()) {
            callback(Result::error("账户名或密码错误"));
            return;
        }

        row.setLatestLoginDate(trantor::Date::now());
        account_mapper.update(row);

        orm::Mapper<Password> password_mapper(client);

        auto enc_data_row = password_mapper.findOne(orm::Criteria(Password::Cols::_id,
                                                                  orm::CompareOperator::EQ, *row.getId()));
        enc_data_row.setEncData(enc_data.asString());
        enc_data_row.setUpdateTime(trantor::Date::now());

        password_mapper.update(enc_data_row);

        callback(Result::ok());
    } catch (orm::UnexpectedRows &e) {
        client->rollback();
        callback(Result::error("账户名或密码错误"));
    }
    catch (...) {
        client->rollback();
        callback(Result::error());
    }
}