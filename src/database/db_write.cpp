#include <db_write.hpp>

InsertOneOperation::InsertOneOperation(std::string &&colName, std::string &&json)
{
    m_json = json;
    m_colName = colName;
}

InsertOneOperation::InsertOneOperation(std::string &&colName, std::string &&json, mongocxx::options::insert insert_opt)
{
    m_json = json;
    m_colName = colName;
    m_has_options = true;
    m_insert_opt = insert_opt;
}

void InsertOneOperation::ExecuteOperation(const mongocxx::database &db)
{
    auto doc_value = bsoncxx::from_json(m_json);
    auto result = db[m_colName].insert_one(doc_value.view());
    //check if the operation was successfull
    if (!result) throw std::exception();
    if (result->result().inserted_count() != 1) 
    {
        m_err = DB_ERR::NO_ACTION_ERROR;
        throw NoActionException("Could not insert, duplicated id");
    }
    m_err = DB_ERR::SUCCESS;
}

void InsertOneOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto doc_value = bsoncxx::from_json(m_json);
    auto result = db[m_colName].insert_one(session, doc_value.view());
    //check if the operation was successfull
    if (!result) throw std::exception();
    if (result->result().inserted_count() != 1) 
    {
        m_err = DB_ERR::NO_ACTION_ERROR;
        throw NoActionException("Could not insert, duplicated id");
    }
    m_err = DB_ERR::SUCCESS;
}

DeleteOneOperation::DeleteOneOperation(std::string &&colName, std::string &&json)
{
    m_json = json;
    m_colName = colName;
}

DeleteOneOperation::DeleteOneOperation(std::string &&colName, std::string &&json, mongocxx::options::delete_options delete_opt)
{
    m_json = json;
    m_colName = colName;
    m_has_options = true;
    m_delete_opt = delete_opt;
}

void DeleteOneOperation::ExecuteOperation(const mongocxx::database &db)
{
    // Convert JSON data to document
    auto doc_value = bsoncxx::from_json(m_json);
    //Delete document
    auto result = db[m_colName].delete_one(doc_value.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}

void DeleteOneOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    // Convert JSON data to document
    auto doc_value = bsoncxx::from_json(m_json);
    //Insert the document
    auto result = db[m_colName].delete_one(session, doc_value.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}

//DELETE_MANY_CLASS
DeleteManyOperation::DeleteManyOperation(std::string &&colName, std::string &&json)
{
    m_json = json;
    m_colName = colName;
}

DeleteManyOperation::DeleteManyOperation(std::string &&colName, std::string &&json, mongocxx::options::delete_options delete_opt)
{
    m_json = json;
    m_colName = colName;
    m_has_options = true;
    m_delete_opt = delete_opt;
}

void DeleteManyOperation::ExecuteOperation(const mongocxx::database &db)
{
    // Convert JSON data to document
    auto doc_value = bsoncxx::from_json(m_json);
    //Delete document
    auto result = db[m_colName].delete_many(doc_value.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}

void DeleteManyOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    // Convert JSON data to document
    auto doc_value = bsoncxx::from_json(m_json);
    //Insert the document
    auto result = db[m_colName].delete_many(session, doc_value.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}