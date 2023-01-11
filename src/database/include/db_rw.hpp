#pragma once
#include <iostream>
#include <mongocxx/database.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/exception/write_exception.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>

enum class RW_ERR
{
    SUCCESS = 0,
    GENERAL_ERR = -1,
    DUPLICATED_ID = -2,
    PARSE_ERR = -3,
};

class TransactionalOperation : public Operation
{
public:
    virtual RW_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) = 0;
};

class InsertOneOperation : public TransactionalOperation 
{
    
public:
    InsertOneOperation(std::string &&colName, std::string &&json)
    {
        m_json = json;
        m_colName = colName;
    }

    
    RW_ERR ExecuteOperation(const mongocxx::database &db) 
    {
        try 
        {
            auto doc_value = bsoncxx::from_json(m_json);

            auto result = db[m_colName].insert_one(doc_value.view());

            //If no documents were added and there was no problem, probably
            //a duplicated id.
            if (result) 
            {
                if (result->result().inserted_count() == 1) return RW_ERR::SUCCESS;
                else return RW_ERR::DUPLICATED_ID;
            }
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
            return RW_ERR::GENERAL_ERR;
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return RW_ERR::PARSE_ERR;
        }
        return RW_ERR::GENERAL_ERR;
    }

    RW_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
    {
        try 
        {
            auto doc_value = bsoncxx::from_json(m_json);

            auto result = db[m_colName].insert_one(session, doc_value.view());

            //If no documents were added and there was no problem, probably
            //a duplicated id.
            if (result) 
            {
                if (result->result().inserted_count() == 1) return RW_ERR::SUCCESS;
                else return RW_ERR::DUPLICATED_ID;
            }
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
            return RW_ERR::GENERAL_ERR;
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return RW_ERR::PARSE_ERR;
        }
        return RW_ERR::GENERAL_ERR;
    }

};

class DeleteOneOperation : public TransactionalOperation
{
public:
    DeleteOneOperation(std::string &&colName, std::string &&json)
    {
        m_json = json;
        m_colName = colName;
    }

    RW_ERR ExecuteOperation(const mongocxx::database &db)  
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(m_json);
            //Insert the document
            db[m_colName].delete_one(doc_value.view());
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return RW_ERR::PARSE_ERR;
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
            return RW_ERR::GENERAL_ERR;
        }
        return RW_ERR::SUCCESS;
    }

    RW_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(m_json);
            //Insert the document
            db[m_colName].delete_one(session, doc_value.view());
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return RW_ERR::PARSE_ERR;
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
            return RW_ERR::GENERAL_ERR;
        }
        return RW_ERR::SUCCESS;
    }
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

    RW_ERR ExecuteTransaction(mongocxx::database &db, mongocxx::v_noabi::client_session &session) 
    {
        for (auto op : m_opList)
        {
            RW_ERR err = op->ExecuteTransactionOperation(db,session);
            if (err != RW_ERR::SUCCESS) return err;
        }

        return RW_ERR::SUCCESS;
    }
};