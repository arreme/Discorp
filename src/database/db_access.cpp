#include <db_access.hpp>

MongoDBAccess::MongoDBAccess(mongocxx::client &client, std::string dbName_) 
: m_client(client)
{
    m_db = m_client[dbName_];
};

FindOneOperation::FindOneOperation(std::string &&colName, std::string &&json)
{
    m_json = json;
    m_colName = colName;
    m_result = "";
}

DB_ERR FindOneOperation::ExecuteOperation(const mongocxx::database &db) 
{
    try 
    {
        // Convert JSON data to document
        auto doc_value = bsoncxx::from_json(m_json);
        //Insert the document
        auto result = db[m_colName].find_one(doc_value.view());
        if (result) 
        {
            m_result = std::move(bsoncxx::to_json(result->view()));
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

bool FindOneOperation::IsResult() 
{
    return !m_result.empty();
}

std::string FindOneOperation::GetResult() 
{
    return m_result;
}