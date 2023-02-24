#pragma once
#include <db_operation.hpp>

class FindOneOperation : public Operation 
{
private:
    bool has_options;
    mongocxx::options::find m_find_opts;
public:
    core::v1::optional<bsoncxx::v_noabi::document::value> result;
    
    FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter);

    void SetOptions(mongocxx::options::find &&find_opts);

    void ExecuteOperation(const mongocxx::database &db) override;
};

class AggregateOperation : public Operation 
{
private:
    mongocxx::pipeline m_p;
    bool m_has_options;
    mongocxx::options::aggregate m_find_opts;
public:
    core::v1::optional<mongocxx::cursor> result;
    
    AggregateOperation(std::string &&colName, mongocxx::pipeline &&pipeline);

    void SetOptions(mongocxx::options::aggregate &&find_opts);

    void ExecuteOperation(const mongocxx::database &db) override;
};