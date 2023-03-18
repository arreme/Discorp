#pragma once
#include <iostream>
#include <db/db_instance.hpp>
#include <db/db_config.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <bsoncxx/json.hpp>

namespace db 
{
    enum class OperationState 
    {
        SUCCESS = 1,
        NOT_EXECUTED = 0,
        PARSE_ERROR = -1,
        BULK_WRITE_ERROR = -2,
        NO_ACTION_ERROR = -3,
        DUPLICATED_ID = -4,
        GENERAL_ERROR = -5,
    };

    class Operation 
    {
    protected:
        std::string m_colName;
        bsoncxx::document::value m_bson;
        OperationState m_db_state = OperationState::NOT_EXECUTED;
    public:
        Operation(std::string &&colName, bsoncxx::v_noabi::document::value &&bson)
        :m_colName(std::forward<std::string>(colName)),m_bson(std::forward<bsoncxx::document::value>(bson)) {};

        OperationState GetState() noexcept { return m_db_state; }

        virtual void ExecuteOperation() noexcept = 0; 
    };

    class TransactionalOperation : public Operation
    {
    public:
        TransactionalOperation(std::string &&colName, bsoncxx::document::value &&bson)
        : Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(bson)) {}; 
        
        virtual void ExecuteOperation(const mongocxx::database &db, const mongocxx::client_session &session) = 0;
    };

    class Transaction 
    {
    protected:
        std::vector<TransactionalOperation *> m_operations;
        OperationState m_db_state = OperationState::NOT_EXECUTED;
    public:

        void AddOperation(TransactionalOperation *op) 
        {
            m_operations.push_back(op);
        }

        void ExecuteTransaction() 
        {
            auto client = MongoDBInstance::GetInstance()->getClientFromPool();
            auto session = client->start_session();
            auto database = (*client)[DATABASE_NAME];
            try
            {
                session.start_transaction();
                for (auto op : m_operations)
                {
                    op->ExecuteOperation(database,session);
                    
                }
                
                session.commit_transaction();
                m_db_state = OperationState::SUCCESS;
            }
            catch (std::exception& e)
            {     
                session.abort_transaction();
                m_db_state = OperationState::GENERAL_ERROR;
            }
        }

        OperationState GetState() noexcept { return m_db_state; }
    };

    class NoActionException : std::runtime_error 
    {
    public:
        NoActionException(const std::string& message) : std::runtime_error(message) 
        {};
    };
}
