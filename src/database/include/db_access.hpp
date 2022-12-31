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

class Operation
{
protected:
    std::string colName;
public:
    virtual void AddToTransaction(mongocxx::database &m_db, mongocxx::v_noabi::client_session &session) = 0;
};

class InsertOneOperation : public Operation 
{
    std::string json;
public:
    InsertOneOperation(std::string collectionName, std::string json) 
    {
        this->json = json;
        colName = collectionName;
    }

    virtual void AddToTransaction(mongocxx::database &m_db, mongocxx::v_noabi::client_session &session) 
    {
        m_db[colName].insert_one(session,bsoncxx::from_json(json));
    }
};

class Transaction 
{
private:
    std::vector<Operation *> opList;
public:
    void AddOperation(Operation *opList) 
    {
        this->opList.push_back(opList);
    }

    void ExecuteTransaction(mongocxx::database &m_db, mongocxx::v_noabi::client_session &session) 
    {
        for (auto op : opList)
        {
            op->AddToTransaction(m_db,session);
        }
    }
};

class MongoDBAccess 
{
private:
    mongocxx::client &m_client;
    std::string m_dbName;
    std::string m_collectionName;
    mongocxx::database m_db;
public:
    MongoDBAccess (mongocxx::client &client, std::string dbName_);

    int InsertOne(std::string &&jsonDoc_, std::string colName);

    int DeleteOne(std::string &&jsonDoc_, std::string colName);

    std::string FindOne(std::string &&jsonDoc_, std::string colName);

    std::vector<std::string> FindMany(std::string &&filter, std::string colName);

    bool MakeTransaction(Transaction &t) 
    {
        auto session = m_client.start_session();
        session.start_transaction();
        
        try
        {
            t.ExecuteTransaction(m_db,session);
        }
        catch (std::exception& e)
        {     
            std::cerr << "Transaction Exception " << e.what() << std::endl;
            session.abort_transaction();
            return false;
        }

        session.commit_transaction();
        return true;
        
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