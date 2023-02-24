#include <db_access.hpp>

MongoDBAccess::MongoDBAccess(mongocxx::client &client, std::string dbName_) 
: m_client(client)
{
    m_db = m_client[dbName_];
};

bool MongoDBAccess::ExecuteOperation(Operation &op) noexcept
{
    try 
    {
        op.ExecuteOperation(m_db);
        return true;
    } catch(bsoncxx::exception e) {
        op.m_err = DB_ERR::PARSE_ERROR;
    } catch(std::exception e) {
        op.m_err = DB_ERR::BULK_WRITE_ERROR;
    }
    return false;
}

bool MongoDBAccess::ExecuteTransaction(Transaction &t) noexcept
{
    auto session = m_client.start_session();
    session.start_transaction();
    try
    {
        t.ExecuteTransaction(m_db, session);
        session.commit_transaction();
        return true;
    }
    catch (std::exception& e)
    {     
        std::cerr << "Transaction Exception " << e.what() << std::endl;
        session.abort_transaction();
    }
    return false;
}