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

class MongoDBAccess 
{
private:
    mongocxx::client &m_client;
    std::string m_dbName;
    std::string m_collectionName;
    mongocxx::database m_db;
    mongocxx::collection m_collection;
public:
    MongoDBAccess (mongocxx::client &client, std::string dbName_, std::string collName_) 
    : m_client(client), m_dbName(dbName_), m_collectionName(collName_) 
    {
        m_db = m_client[dbName_];
        m_collection = m_db[collName_];
    }

    int insert_one (std::string &&jsonDoc_) 
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(jsonDoc_);
            //Insert the document
            auto result = m_collection.insert_one(std::move(doc_value));
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return -1;
        }
        catch (mongocxx::write_exception e) 
        {
            std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
            return -1;
        }
        return 0;
    }

    int delete_one(std::string &&jsonDoc_) 
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(jsonDoc_);
            //Insert the document
            auto result = m_collection.delete_one(std::move(doc_value));
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
            return -1;
        }
        catch (mongocxx::write_exception e) 
        {
            std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
            return -1;
        }
        return 0;
    }

    std::string find_one(std::string &&jsonDoc_) 
    {
        try 
        {
            // Convert JSON data to document
            auto doc_value = bsoncxx::from_json(jsonDoc_);
            //Insert the document
            bsoncxx::stdx::optional<bsoncxx::document::value> result = m_collection.find_one(std::move(doc_value));
            if (result) 
            {
                return bsoncxx::to_json(result->view());
            }
        }
        catch(const bsoncxx::exception& e) 
        {
            std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
        }
        catch (mongocxx::query_exception e) 
        {
            std::string errInfo = std::string("Error in query operation, Err Msg : ") + e.what();
        }
        return {};
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