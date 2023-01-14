#pragma once
#include <iostream>
#include <mongocxx/pool.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/exception/write_exception.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <db_write.hpp>

class MongoDBAccess 
{
private:
    mongocxx::client &m_client;
    mongocxx::database m_db;
public:
    MongoDBAccess (mongocxx::client &client, std::string dbName_);

    bool ExecuteOperation(Operation &op) noexcept;

    bool ExecuteTransaction(Transaction &t) noexcept;

    core::v1::optional<bsoncxx::v_noabi::document::value> FindOne(std::string &&collection, bsoncxx::document::value filter, const mongocxx::options::find & options = mongocxx::options::find()) noexcept;
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