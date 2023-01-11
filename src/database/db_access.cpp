#include <db_access.hpp>

MongoDBAccess::MongoDBAccess(mongocxx::client &client, std::string dbName_) 
: m_client(client)
{
    m_db = m_client[dbName_];
};

RW_ERR MongoDBAccess::ExecuteOperation(Operation &op)
{
    return op.ExecuteOperation(m_db);
}

RW_ERR MongoDBAccess::ExecuteTransaction(Transaction &t) 
{
    auto session = m_client.start_session();
    session.start_transaction();
    RW_ERR err;
    try
    {
        err = t.ExecuteTransaction(m_db, session);
        if (err != RW_ERR::SUCCESS) 
        {
            session.abort_transaction();
            return err;
        }
    }
    catch (std::exception& e)
    {     
        std::cerr << "Transaction Exception " << e.what() << std::endl;
        session.abort_transaction();
        return RW_ERR::GENERAL_ERR;
    }

    session.commit_transaction();
    return RW_ERR::SUCCESS;
}