#include <db_query.hpp>

FindOneOperation::FindOneOperation(std::string &&colName, bsoncxx::document::value &&filter)
: Operation(std::forward<std::string>(colName),std::forward<bsoncxx::document::value>(filter))
{}

void FindOneOperation::SetOptions(mongocxx::options::find &&find_opts) 
{
    has_options = true;
    m_find_opts = find_opts;
}

void FindOneOperation::ExecuteOperation(const mongocxx::database &db) 
{
    if (has_options)
    {
        result = db[m_colName].find_one(m_bson.view(),m_find_opts);
    } else 
    {
        result = db[m_colName].find_one(m_bson.view());
    }
}