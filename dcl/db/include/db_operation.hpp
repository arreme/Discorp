#pragma once
#include <iostream>
#include <db_instance.hpp>
#include <db_config.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>

enum class OperationState 
{
    SUCCESS = 1,
    NOT_EXECUTED = 0,
    PARSE_ERROR = -1,
    BULK_WRITE_ERROR = -2,
    NO_ACTION_ERROR = -3,
};

class Operation 
{
protected:
    std::string m_colName;
    bsoncxx::document::value m_bson;
    OperationState m_db_state = OperationState::NOT_EXECUTED;
public:
    Operation(std::string &&colName, bsoncxx::v_noabi::document::value &&bson)
    :m_colName(colName),m_bson(bson) {};

    OperationState GetState() { return m_db_state; }

    virtual void ExecuteOperation(const mongocxx::database &db) = 0;
    virtual void ExecuteOperation(const mongocxx::database &db, const mongocxx::client_session &session) = 0;
};

class Transaction 
{
protected:
    std::vector<Operation *> m_operations;
public:
    bool ExecuteTransaction() 
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto session = client->start_session();
        auto database = (*client)[DATABASE_NAME];
        session.start_transaction();
        try
        {
            for (auto op : m_operations)
            {
                op->ExecuteOperation(database,session);
            }
            
            session.commit_transaction();
            
        }
        catch (std::exception& e)
        {     
            std::cerr << "Transaction Exception " << e.what() << std::endl;
            session.abort_transaction();
            return false;
        }
        return true;
    }
};

// class NoActionException : std::runtime_error 
// {
// public:
//     NoActionException(const std::string& message) : std::runtime_error(message) 
//     {};
// };