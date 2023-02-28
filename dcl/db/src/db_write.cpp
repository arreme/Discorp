#include <db_write.hpp>

/**************************
 *  INSERT ONE OPERATION  *
 **************************/

InsertOneOperation::InsertOneOperation(std::string &&colName, std::unique_ptr<document::value> bson)
: TransactionalOperation(std::forward<std::string>(colName),bson)
{}

InsertOneOperation::InsertOneOperation(std::string &&colName, bsoncxx::document::value &&bson, mongocxx::options::insert insert_opt)
: TransactionalOperation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(bson))
{
    m_has_options = true;
    m_insert_opt = insert_opt;
}

void InsertOneOperation::ExecuteOperation() noexcept
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];

        auto result = db[m_colName].insert_one(m_bson.view());
        m_db_state = OperationState::SUCCESS;

        //check if the operation was successfull
        if (!result) 
        {
            m_db_state = OperationState::NOT_EXECUTED;
        } else if (result->result().inserted_count() != 1) 
        {
            m_db_state = OperationState::DUPLICATED_ID;
        }
    }
    catch(const mongocxx::bulk_write_exception e) {
        m_db_state = OperationState::BULK_WRITE_ERROR;
    } catch (const std::exception e) {
        m_db_state = OperationState::GENERAL_ERROR;
    }
    
}

void InsertOneOperation::ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto result = db[m_colName].insert_one(session, m_bson.view());
    //check if the operation was successfull

    if (!result) throw std::exception();

    if (result->result().inserted_count() != 1) 
    {
        m_db_state = OperationState::DUPLICATED_ID;
        throw NoActionException("Could not insert, duplicated id");
    }

    m_db_state = OperationState::SUCCESS;
}

/**************************
 *  DELETE ONE OPERATION  *
 **************************/

DeleteOneOperation::DeleteOneOperation(std::string &&colName, bsoncxx::document::value &&bson)
: TransactionalOperation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(bson))
{}

void DeleteOneOperation::ExecuteOperation() noexcept
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];

        auto result = db[m_colName].delete_one(m_bson.view());
        m_db_state = OperationState::SUCCESS;
        if (!result) 
        {
            //No operation was returned
            OperationState::NOT_EXECUTED;
        } else if (result->deleted_count() == 0) 
        {
            //The delete was not successful
            OperationState::NOT_EXECUTED;
        }
    }
    catch(const mongocxx::bulk_write_exception e)
    {
        m_db_state = OperationState::BULK_WRITE_ERROR;
    } catch (const std::exception e) {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

void DeleteOneOperation::ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    //Insert the document
    auto result = db[m_colName].delete_one(session, m_bson.view());

    if (!result) throw std::exception();

    m_db_state = OperationState::SUCCESS;

    if (result->deleted_count() == 0) 
    {
        m_db_state = OperationState::NO_ACTION_ERROR;
    }
    
}

/***************************
 *  DELETE MANY OPERATION  *
 ***************************/
DeleteManyOperation::DeleteManyOperation(std::string &&colName, bsoncxx::document::value &&bson)
: TransactionalOperation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(bson))
{}

void DeleteManyOperation::ExecuteOperation() noexcept
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];

        auto result = db[m_colName].delete_many(m_bson.view());
        m_db_state = OperationState::SUCCESS;
        if (!result) 
        {
            //No operation was returned
            OperationState::NOT_EXECUTED;
        } else if (result->deleted_count() == 0) 
        {
            //The delete was not successful
            OperationState::NOT_EXECUTED;
        }
    }
    catch(const mongocxx::bulk_write_exception e)
    {
        m_db_state = OperationState::BULK_WRITE_ERROR;
    } catch (const std::exception e) {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

void DeleteManyOperation::ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto result = db[m_colName].delete_many(session, m_bson.view());

    if (!result) throw std::exception();

    m_db_state = OperationState::SUCCESS;

    if (result->deleted_count() == 0) 
    {
        m_db_state = OperationState::NO_ACTION_ERROR;
    }
}

/**************************
 *  UPDATE ONE OPERATION  *
 **************************/

UpdateOneOperation::UpdateOneOperation(std::string &&colName, bsoncxx::document::value &&filter, bsoncxx::document::value &&update_query) 
: TransactionalOperation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter)), m_update_query(std::forward<bsoncxx::document::value>(update_query))
{}

void UpdateOneOperation::ExecuteOperation() noexcept
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];

        auto result = db[m_colName].update_one(m_bson.view(), m_update_query.view());
        m_db_state = OperationState::SUCCESS;
        if (!result) 
        {
            //No operation was returned
            OperationState::NOT_EXECUTED;
        } else if (result->modified_count() == 0) 
        {
            //The delete was not successful
            OperationState::NOT_EXECUTED;
        }
    }
    catch(const mongocxx::bulk_write_exception e)
    {
        m_db_state = OperationState::BULK_WRITE_ERROR;
    } catch (const std::exception e) {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

void UpdateOneOperation::ExecuteOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session)
{
    auto result = db[m_colName].update_one(session, m_bson.view(),m_update_query.view());
    if (!result) throw std::exception();

    m_db_state = OperationState::SUCCESS;

    if (result->modified_count() == 0) 
    {
        m_db_state = OperationState::NO_ACTION_ERROR;
    }
}
