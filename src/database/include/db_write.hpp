#pragma once
#include <iostream>
#include <mongocxx/database.hpp>
#include <bsoncxx/json.hpp>
#include <db_operation.hpp>

class TransactionalOperation : public Operation
{
public:
    TransactionalOperation(std::string &&colName, bsoncxx::v_noabi::document::value bson)
    : Operation(std::forward<std::string>(colName),bson) {}; 
    virtual void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) = 0;
};

class InsertOneOperation : public TransactionalOperation 
{
private:
    mongocxx::options::insert m_insert_opt;
    bool m_has_options;
public:
    InsertOneOperation(std::string &&colName, bsoncxx::v_noabi::document::value bson);

    InsertOneOperation(std::string &&colName, bsoncxx::v_noabi::document::value bson, mongocxx::options::insert insert_opt);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteOneOperation : public TransactionalOperation
{
private:
    mongocxx::options::delete_options m_delete_opt;
    bool m_has_options;
public:
    DeleteOneOperation(std::string &&colName, bsoncxx::document::value bson);

    DeleteOneOperation(std::string &&colName, bsoncxx::document::value bson, mongocxx::options::delete_options delete_opt);

    void ExecuteOperation(const mongocxx::database &db) override;

    void ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) override;
};

class DeleteManyOperation : public TransactionalOperation 
{
private:
    mongocxx::options::delete_options m_delete_opt;
    bool m_has_options;
public:
    DeleteManyOperation(std::string &&colName, bsoncxx::document::value bson);

    DeleteManyOperation(std::string &&colName, bsoncxx::document::value bson, mongocxx::options::delete_options delete_opt);

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