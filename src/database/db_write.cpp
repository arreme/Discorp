#include <db_write.hpp>

InsertOneOperation::InsertOneOperation(std::string &&colName, bsoncxx::document::value bson)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{}

InsertOneOperation::InsertOneOperation(std::string &&colName, bsoncxx::document::value bson, mongocxx::options::insert insert_opt)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{
    m_has_options = true;
    m_insert_opt = insert_opt;
}

void InsertOneOperation::ExecuteOperation(const mongocxx::database &db)
{
    auto result = db[m_colName].insert_one(m_bson.view());
    //check if the operation was successfull
    if (!result) 
    {
        throw std::exception();
    }
    if (result->result().inserted_count() != 1) 
    {
        m_err = DB_ERR::NO_ACTION_ERROR;
        throw NoActionException("Could not insert, duplicated id");
    }
    m_err = DB_ERR::SUCCESS;
}

void InsertOneOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto result = db[m_colName].insert_one(session, m_bson.view());
    //check if the operation was successfull
    if (!result) throw std::exception();
    if (result->result().inserted_count() != 1) 
    {
        m_err = DB_ERR::NO_ACTION_ERROR;
        throw NoActionException("Could not insert, duplicated id");
    }
    m_err = DB_ERR::SUCCESS;
}

DeleteOneOperation::DeleteOneOperation(std::string &&colName, bsoncxx::document::value bson)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{}

DeleteOneOperation::DeleteOneOperation(std::string &&colName, bsoncxx::document::value bson, mongocxx::options::delete_options delete_opt)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{
    m_has_options = true;
    m_delete_opt = delete_opt;
}

void DeleteOneOperation::ExecuteOperation(const mongocxx::database &db)
{
    //Delete document
    auto result = db[m_colName].delete_one(m_bson.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}

void DeleteOneOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    //Insert the document
    auto result = db[m_colName].delete_one(session, m_bson.view());
    if (!result) throw std::exception();
    //If it hasn't deleted any document, throw
    m_err = DB_ERR::SUCCESS;
}

//DELETE_MANY_CLASS
DeleteManyOperation::DeleteManyOperation(std::string &&colName, bsoncxx::document::value bson)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{}

DeleteManyOperation::DeleteManyOperation(std::string &&colName, bsoncxx::document::value bson, mongocxx::options::delete_options delete_opt)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{
    m_has_options = true;
    m_delete_opt = delete_opt;
}

void DeleteManyOperation::ExecuteOperation(const mongocxx::database &db)
{
    //Delete document
    auto result = db[m_colName].delete_many(m_bson.view());
    if (!result) throw std::exception();

    //If there is no result, something happened.
    m_err = DB_ERR::SUCCESS;
}

void DeleteManyOperation::ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto result = db[m_colName].delete_many(m_bson.view());
    if (!result) throw std::exception();

    //If there is no result, something happened.
    m_err = DB_ERR::SUCCESS;
}