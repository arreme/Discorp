#pragma once
#include <iostream>
#include <mongocxx/database.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <bsoncxx/json.hpp>
#include <db_operation.hpp>
#include <db_instance.hpp>
#include <db_config.hpp>

using namespace bsoncxx::v_noabi;

class InsertOneOperation : public TransactionalOperation 
{
private:
    mongocxx::options::insert m_insert_opt;
    bool m_has_options;
public:
    InsertOneOperation(std::string &&colName, std::unique_ptr<document::value> bson);

    InsertOneOperation(std::string &&colName, document::value &&bson, mongocxx::options::insert insert_opt);

    void ExecuteOperation() noexcept override;

    void ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteOneOperation : public TransactionalOperation
{
public:
    DeleteOneOperation(std::string &&colName, bsoncxx::document::value &&bson);

    void ExecuteOperation() noexcept override;

    void ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteManyOperation : public TransactionalOperation 
{
public:
    DeleteManyOperation(std::string &&colName, bsoncxx::document::value &&bson);

    void ExecuteOperation() noexcept override;

    void ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class UpdateOneOperation : public TransactionalOperation 
{
private:
    bsoncxx::document::value m_update_query;
    mongocxx::options::update m_update_opts;
    bool m_has_options;
public:
    UpdateOneOperation(std::string &&colName, bsoncxx::document::value &&filter, bsoncxx::document::value &&update_query);

    void ExecuteOperation() noexcept override;

    void ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};