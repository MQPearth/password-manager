/**
 *
 *  Account.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/BaseBuilder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace password
{

class Account
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _login_name;
        static const std::string _password;
        static const std::string _create_date;
        static const std::string _latest_login_date;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = std::string;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Account(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Account(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Account(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Account() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const std::string &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const std::string &pId) noexcept;
    void setId(std::string &&pId) noexcept;

    /**  For column login_name  */
    ///Get the value of the column login_name, returns the default value if the column is null
    const std::string &getValueOfLoginName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getLoginName() const noexcept;
    ///Set the value of the column login_name
    void setLoginName(const std::string &pLoginName) noexcept;
    void setLoginName(std::string &&pLoginName) noexcept;

    /**  For column password  */
    ///Get the value of the column password, returns the default value if the column is null
    const std::string &getValueOfPassword() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPassword() const noexcept;
    ///Set the value of the column password
    void setPassword(const std::string &pPassword) noexcept;
    void setPassword(std::string &&pPassword) noexcept;

    /**  For column create_date  */
    ///Get the value of the column create_date, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreateDate() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreateDate() const noexcept;
    ///Set the value of the column create_date
    void setCreateDate(const ::trantor::Date &pCreateDate) noexcept;

    /**  For column latest_login_date  */
    ///Get the value of the column latest_login_date, returns the default value if the column is null
    const ::trantor::Date &getValueOfLatestLoginDate() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getLatestLoginDate() const noexcept;
    ///Set the value of the column latest_login_date
    void setLatestLoginDate(const ::trantor::Date &pLatestLoginDate) noexcept;


    static size_t getColumnNumber() noexcept {  return 5;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Account>;
    friend drogon::orm::BaseBuilder<Account, true, true>;
    friend drogon::orm::BaseBuilder<Account, true, false>;
    friend drogon::orm::BaseBuilder<Account, false, true>;
    friend drogon::orm::BaseBuilder<Account, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Account>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> id_;
    std::shared_ptr<std::string> loginName_;
    std::shared_ptr<std::string> password_;
    std::shared_ptr<::trantor::Date> createDate_;
    std::shared_ptr<::trantor::Date> latestLoginDate_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[5]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = ?";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = ?";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        if(dirtyFlag_[0])
        {
            sql += "id,";
            ++parametersCount;
        }
        if(dirtyFlag_[1])
        {
            sql += "login_name,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "password,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "create_date,";
            ++parametersCount;
        }
        if(dirtyFlag_[4])
        {
            sql += "latest_login_date,";
            ++parametersCount;
        }
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        if(dirtyFlag_[0])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[1])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[2])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[3])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[4])
        {
            sql.append("?,");

        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace password
} // namespace drogon_model
