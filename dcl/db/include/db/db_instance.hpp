#pragma once
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp> 
#include <iostream>
#include <memory>

namespace db 
{
    class MongoDBInstance 
    {
    private:
        mongocxx::instance m_dbInstance;
        std::unique_ptr<mongocxx::pool> m_client_pool;
        MongoDBInstance () { }
    public:
        static MongoDBInstance* GetInstance () 
        {
            static MongoDBInstance objMongoDBInstance;
            return &objMongoDBInstance;
        }

        /* Create a pool object only once from MongoDB URI */
        void createPool(std::string uri) 
        {
            if (!m_client_pool)
            {
                m_client_pool = std::unique_ptr<mongocxx::v_noabi::pool>{new mongocxx::pool { mongocxx::uri {uri} }};
            }
        }
        
        /* Acquire a client from the pool */
        mongocxx::pool::entry getClientFromPool () 
        { 
            return m_client_pool->acquire(); 
        }

    };
}
