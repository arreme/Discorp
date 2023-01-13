#pragma once
#include <iostream>
#include <mongocxx/database.hpp>

enum class DB_ERR 
{
    SUCCESS = 1,
    NOT_EXECUTED = 0,
    PARSE_ERROR = -1,
    BULK_WRITE_ERROR = -2,
    NO_ACTION_ERROR = -3,
};

class Operation 
{
protected:
    std::string m_colName;
    std::string m_json;


public:
    DB_ERR m_err = DB_ERR::NOT_EXECUTED;
    virtual void ExecuteOperation(const mongocxx::database &db) = 0;
};

class NoActionException : std::runtime_error 
{
public:
    NoActionException(const std::string& message) : std::runtime_error(message) 
    {};
};