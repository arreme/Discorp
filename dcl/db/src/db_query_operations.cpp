#include <db/db_query_operations.hpp>

using namespace db;

/************************
 *  FIND ONE OPERATION  *
 ************************/

FindOneOperation::FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter)
: Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter))
{}

FindOneOperation::FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter, mongocxx::options::find  &&opt)
: FindOneOperation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter))
{
    SetOptions(std::forward<mongocxx::options::find>(opt));
}

void FindOneOperation::SetOptions(mongocxx::options::find &&find_opts) 
{
    m_has_options = true;
    m_find_opts = find_opts;
}

void FindOneOperation::ExecuteOperation() noexcept 
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];
        if (m_has_options)
        {
            m_result = db[m_colName].find_one(m_bson.view(), m_find_opts);
        } else 
        {
            m_result = db[m_colName].find_one(m_bson.view());
        }
        m_db_state = OperationState::SUCCESS;
    }
    catch(std::exception e)
    {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

/*************************
 *  FIND MANY OPERATION  *
 *************************/

FindManyOperation::FindManyOperation(std::string &&colName, bsoncxx::document::value &&filter)
: Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter))
{}

void FindManyOperation::SetOptions(mongocxx::options::find &&find_opts) 
{
    m_has_options = true;
    m_find_opts = find_opts;
}

void FindManyOperation::ExecuteOperation() noexcept
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];
        
        if (m_has_options)
        {
            m_result = db[m_colName].find(m_bson.view(), m_find_opts);
        } else 
        {
            m_result = db[m_colName].find(m_bson.view());
        }
        m_db_state = OperationState::SUCCESS;
    }
    catch(std::exception e)
    {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

std::optional<bsoncxx::document::view> FindManyOperation::GetResult() noexcept
{
    try 
    {
        if (m_result)
        {
            auto iter = m_result->begin();
            if (iter == m_result->end()) {
                //Iterator is exhausted
                m_result = std::nullopt;
                return std::nullopt;
            } 
            return *(iter);
        }
        
    } catch ( mongocxx::query_exception e ) {
        
    }
    return std::nullopt;
}

/*************************
 *  AGGREGATE OPERATION  *
 *************************/

AggregateOperation::AggregateOperation(std::string &&colName, mongocxx::pipeline &&p)
: Operation(std::forward<std::string>(colName),bsoncxx::builder::basic::make_document())
{
    m_p = std::move(p);
}

void AggregateOperation::SetOptions(mongocxx::options::aggregate &&find_opts) 
{
    m_has_options = true;
    m_find_opts = find_opts;
}

void AggregateOperation::ExecuteOperation() noexcept 
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];
        
        if (m_has_options)
        {
            m_result = db[m_colName].aggregate(m_p, m_find_opts);
        } else 
        {
            m_result = db[m_colName].aggregate(m_p);
        }
        m_db_state = OperationState::SUCCESS;
    }
    catch(std::exception e)
    {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}

std::optional<bsoncxx::document::view> AggregateOperation::GetResult() noexcept
{
    try 
    {
        if (m_result)
        {
            auto iter = m_result->begin();          
            if (iter == m_result->end()) {
                //Iterator is exhausted
                m_result = std::nullopt;
                return std::nullopt;
            }
            return *(iter);
        }
        
    } catch ( mongocxx::query_exception e ) {
        
    }
    return std::nullopt;
}

/***********************************
 *  FIND ONE AND UPDATE OPERATION  *
 ***********************************/

FindOneAndUpdateOperation::FindOneAndUpdateOperation(std::string &&colName, bsoncxx::document::value &&filter, bsoncxx::document::value &&update_bson)
: Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter)) , m_update_bson(std::forward<bsoncxx::document::value>(update_bson))
{}

void FindOneAndUpdateOperation::ExecuteOperation() noexcept 
{
    try
    {
        auto client = MongoDBInstance::GetInstance()->getClientFromPool();
        auto db = (*client)[DATABASE_NAME];
        
        m_result = db[m_colName].find_one_and_update(m_bson.view(),m_update_bson.view());

        m_db_state = OperationState::SUCCESS;
    }
    catch(std::exception e)
    {
        m_db_state = OperationState::GENERAL_ERROR;
    }
}
