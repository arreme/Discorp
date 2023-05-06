#pragma once
#include <db/db_operation.hpp>
#include <mongocxx/exception/query_exception.hpp>

namespace db 
{
    class FindOneOperation : public Operation 
    {
    private:
        bool m_has_options;
        mongocxx::options::find m_find_opts;
    public:
        std::optional<bsoncxx::document::value> m_result;
        
        FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter);
        FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter, mongocxx::options::find  &&opt);
        void SetOptions(mongocxx::options::find &&find_opts);

        void ExecuteOperation() noexcept override;
    };

    class FindManyOperation : public Operation 
    {
    private:
        bool m_has_options;
        mongocxx::options::find m_find_opts;
        std::optional<mongocxx::cursor> m_result;
    public:
        
        FindManyOperation(std::string &&colName, bsoncxx::document::value &&filter);

        void SetOptions(mongocxx::options::find &&find_opts);

        void ExecuteOperation() noexcept override;

        std::optional<bsoncxx::document::view> GetResult() noexcept;
    };

    class AggregateOperation : public Operation 
    {
    private:
        mongocxx::pipeline m_p;
        bool m_has_options;
        mongocxx::options::aggregate m_find_opts;
        std::optional<mongocxx::cursor> m_result;
    public:
        AggregateOperation(std::string &&colName, mongocxx::pipeline &&p);

        void SetOptions(mongocxx::options::aggregate &&find_opts);

        void ExecuteOperation() noexcept override;

        std::optional<bsoncxx::document::view> GetResult() noexcept;
    };

    class FindOneAndUpdateOperation : public Operation 
    {
    private:
        bsoncxx::document::value m_update_bson;
    public:
        std::optional<bsoncxx::document::value> m_result;

        FindOneAndUpdateOperation(std::string &&colName, bsoncxx::document::value &&filter, bsoncxx::document::value &&update_bson);
        
        void ExecuteOperation() noexcept override;
    };
}
