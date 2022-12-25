#include <db_access.hpp>

MongoDBAccess::MongoDBAccess(mongocxx::client &client, std::string dbName_, std::string collName_) 
: m_client(client), m_dbName(dbName_), m_collectionName(collName_) 
{
    m_db = m_client[dbName_];
    m_collection = m_db[collName_];
};

int MongoDBAccess::InsertOne(std::string &&jsonDoc_) 
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
    catch (mongocxx::bulk_write_exception e) 
    {
        std::string errInfo = std::string("Error in inserting document, Err Msg : ") + e.what();
        return -1;
    }
    return 0;
};

int MongoDBAccess::DeleteOne(std::string &&jsonDoc_) 
{
    try 
    {
        // Convert JSON data to document
        auto doc_value = bsoncxx::from_json(jsonDoc_);
        //Insert the document
        auto result = m_collection.delete_one(std::move(doc_value));
        if (result) 
        {
            return result->deleted_count();
        }
    }
    catch(const bsoncxx::exception& e) 
    {
        std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
    }
    catch (mongocxx::bulk_write_exception e) 
    {
        std::string errInfo = std::string("Error in deleting document, Err Msg : ") + e.what();
    }
    return {};
};

std::string MongoDBAccess::FindOne(std::string &&jsonDoc_) 
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
};

std::vector<std::string> MongoDBAccess::FindMany(std::string &&filter)  
{
    try 
    {
        // Convert JSON data to document
        auto doc_value = bsoncxx::from_json(filter);
        //Insert the document
        mongocxx::cursor result = m_collection.find(std::move(doc_value));
        
        std::vector<std::string> documents;

        for (auto doc : result) 
        {
            documents.push_back(bsoncxx::to_json(doc));
        }

        return documents;

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
};