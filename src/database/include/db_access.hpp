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
#include <optional>

typedef enum
{
    SUCCESS = 0,
    GENERAL_ERR = -1,
    DUPLICATED_ID = -2,
    PARSE_ERR = -3,
} DB_ERR;

class Operation
{
protected:
    std::string m_colName;
    std::string m_json;
public:
    virtual DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) = 0;
    virtual DB_ERR ExecuteOperation(const mongocxx::database &db) = 0;
};

class InsertOneOperation : public Operation 
{
    
public:
    InsertOneOperation(std::string &&colName, std::string &&json)
    {
        m_json = json;
        m_colName = colName;
    }

    
    DB_ERR ExecuteOperation(const mongocxx::database &db) 
    {
        try 
        {
            auto doc_value = bsoncxx::from_json(m_json);

            auto result = db[m_colName].insert_one(doc_value.view());

            //If no documents were added and there was no problem, probably
            //a duplicated id.
            if (result) 
            {
                if (result->result().inserted_count() == 1) return SUCCESS;
                else return DUPLICATED_ID;
            }
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
            return GENERAL_ERR;
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return PARSE_ERR;
        }
        return GENERAL_ERR;
    }

    DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
    {
        try 
        {
            auto doc_value = bsoncxx::from_json(m_json);

            auto result = db[m_colName].insert_one(session, doc_value.view());

            //If no documents were added and there was no problem, probably
            //a duplicated id.
            if (result) 
            {
                if (result->result().inserted_count() == 1) return SUCCESS;
                else return DUPLICATED_ID;
            }
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
            return GENERAL_ERR;
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return PARSE_ERR;
        }
        return GENERAL_ERR;
    }

};

class DeleteOneOperation : public Operation
{
public:
    DeleteOneOperation(std::string &&colName, std::string &&json)
    {
        m_json = json;
        m_colName = colName;
    }

    DB_ERR ExecuteOperation(const mongocxx::database &db)  
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
            return PARSE_ERR;
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
            return GENERAL_ERR;
        }
        return SUCCESS;
    }

    DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
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
            return PARSE_ERR;
        }
        catch (mongocxx::bulk_write_exception e) 
        {
            std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
            return GENERAL_ERR;
        }
        return SUCCESS;
    }
};

class FindOneOperation : public Operation
{
private:
    std::string m_result;
public:
    FindOneOperation(std::string &&colName, std::string &&json);

    DB_ERR ExecuteOperation(const mongocxx::database &db);

    DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
    {
        //Should not be able to call this... maybe use two different classes?
        return SUCCESS;
    }

    bool IsResult();

    std::string GetResult();
};

class FindManyOperation : public Operation
{
private:
    std::vector<std::string> m_result;
public:
    DB_ERR ExecuteOperation(const mongocxx::database &db) 
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(m_json);
            //Insert the document
            mongocxx::cursor result = db[m_colName].find(doc_value.view());

            for (auto doc : result) 
            {
                m_result.push_back(bsoncxx::to_json(doc));
            }
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return PARSE_ERR;
        }
        catch (mongocxx::query_exception e) 
        {
            std::string errInfo = std::string("Error in query operation, Err Msg : ") + e.what();
            return GENERAL_ERR;
        }
        return SUCCESS;
    }

    DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
    {
        //Should not be able to call this... maybe use two different classes?
        return SUCCESS;
    }



};

class Transaction 
{
private:
    std::vector<Operation *> m_opList;
public:
    void AddOperation(Operation &op) 
    {
        this->m_opList.push_back(&op);
    }

    DB_ERR ExecuteTransaction(mongocxx::database &db, mongocxx::v_noabi::client_session &session) 
    {
        for (auto op : m_opList)
        {
            DB_ERR err = op->ExecuteTransactionOperation(db,session);
            if (err != SUCCESS) return err;
        }

        return SUCCESS;
    }
};

class MongoDBAccess 
{
private:
    mongocxx::client &m_client;
    mongocxx::database m_db;
public:
    MongoDBAccess (mongocxx::client &client, std::string dbName_);

    DB_ERR ExecuteOperation(Operation &op) 
    {
        return op.ExecuteOperation(m_db);
    }

    DB_ERR ExecuteTransaction(Transaction &t) 
    {
        auto session = m_client.start_session();
        session.start_transaction();
        DB_ERR err;
        try
        {
            err = t.ExecuteTransaction(m_db, session);
            if (err != SUCCESS) 
            {
                session.abort_transaction();
                return err;
            }
        }
        catch (std::exception& e)
        {     
            std::cerr << "Transaction Exception " << e.what() << std::endl;
            session.abort_transaction();
            return GENERAL_ERR;
        }

        session.commit_transaction();
        return SUCCESS;

    }
};

// class MongoDBThread 
// {
// private:
//     MongoDBAccess objDB_access;
//     std::string   strJSON_doc;
// public:
//     MongoDBThread(mongocxx::client& client, std::string db_name, std::string coll_name, std::string json_doc) 
//     : objDB_access(client, db_name, coll_name), strJSON_doc(json_doc)
//     {

//     }
//     void operator()() 
//     {
//         objDB_access.insert(strJSON_doc);
//     }
// };