#pragma once
#include <iostream>
#include <mongocxx/database.hpp>
#include <bsoncxx/json.hpp>
#include <db_operation.hpp>

class TransactionalOperation : public Operation
{
public:
    TransactionalOperation(std::string &&colName, bsoncxx::v_noabi::document::value &&bson)
    : Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(bson)) {}; 
    virtual void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) = 0;
};

class InsertOneOperation : public TransactionalOperation 
{
private:
    mongocxx::options::insert m_insert_opt;
    bool m_has_options;
public:
    InsertOneOperation(std::string &&colName, bsoncxx::v_noabi::document::value &&bson);

    InsertOneOperation(std::string &&colName, bsoncxx::v_noabi::document::value &&bson, mongocxx::options::insert insert_opt);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteOneOperation : public TransactionalOperation
{
public:
    DeleteOneOperation(std::string &&colName, bsoncxx::document::value &&bson);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteManyOperation : public TransactionalOperation 
{
public:
    DeleteManyOperation(std::string &&colName, bsoncxx::document::value &&bson);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class UpdateOneOperation : public TransactionalOperation 
{
private:
    bsoncxx::document::value m_update_query;
    mongocxx::options::update m_update_opts;
    bool m_has_options;
public:
    UpdateOneOperation(std::string &&colName, bsoncxx::document::value &&filter, bsoncxx::document::value &&update_query);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class Transaction 
{
private:
    std::vector<TransactionalOperation *> m_opList;
public:
    void AddOperation(TransactionalOperation &op) 
    {
        this->m_opList.push_back(&op);
    }

    void ExecuteTransaction(mongocxx::database &db, mongocxx::v_noabi::client_session &session) 
    {
        for (auto op : m_opList)
        {
            op->ExecuteTransactionOperation(db,session);
        }
    }
};