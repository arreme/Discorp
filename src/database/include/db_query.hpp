// class FindOneOperation : public Operation
// {
// private:
//     std::string m_result;
// public:
//     FindOneOperation(std::string &&colName, std::string &&json);

//     DB_ERR ExecuteOperation(const mongocxx::database &db);

//     DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
//     {
//         //Should not be able to call this... maybe use two different classes?
//         return SUCCESS;
//     }

//     bool IsResult();

//     std::string GetResult();
// };

// class FindManyOperation : public Operation
// {
// private:
//     std::vector<std::string> m_result;
// public:
//     DB_ERR ExecuteOperation(const mongocxx::database &db) 
//     {
//         try 
//         {
//             // Convert JSON data to document
//             auto doc_value = bsoncxx::from_json(m_json);
//             //Insert the document
//             mongocxx::cursor result = db[m_colName].find(doc_value.view());

//             for (auto doc : result) 
//             {
//                 m_result.push_back(bsoncxx::to_json(doc));
//             }
//         }
//         catch(const bsoncxx::exception& e) 
//         {
//             std::string errInfo = std::string("Error in converting JSONdata,Err Msg : ") + e.what();
//             return PARSE_ERR;
//         }
//         catch (mongocxx::query_exception e) 
//         {
//             std::string errInfo = std::string("Error in query operation, Err Msg : ") + e.what();
//             return GENERAL_ERR;
//         }
//         return SUCCESS;
//     }

//     DB_ERR ExecuteTransactionOperation(const mongocxx::database &db, const mongocxx::v_noabi::client_session &session) 
//     {
//         //Should not be able to call this... maybe use two different classes?
//         return SUCCESS;
//     }



// };