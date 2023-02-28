#include <catch2/catch_test_macros.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <db_operation.hpp>
#include <db_write.hpp>
#include <db_query_operations.hpp>

using bsoncxx::builder::basic::kvp;

SCENARIO("Testing database basic operations","[db]") 
{
    GIVEN("1.- A new empty user collection collection") 
    {
        DeleteManyOperation del_op_1 = DeleteManyOperation("users", bsoncxx::builder::basic::make_document());
        del_op_1.ExecuteOperation();
        
        WHEN("1.1.- FindOne Operation") 
        {
            FindOneOperation find_op_1_1 = FindOneOperation("users",bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("discord_id",0)));
            find_op_1_1.ExecuteOperation();
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(find_op_1_1.m_result);
            }
        }
        WHEN("1.2.- Adding document to the collection") 
        {
            uint64_t id = 0;

            InsertOneOperation ins_op_1_2 = InsertOneOperation("test_col", 
                bsoncxx::builder::basic::make_document(
                    bsoncxx::builder::basic::kvp("id",id),
                    bsoncxx::builder::basic::kvp("name","John Doe")
                ).release()
            );
            
            THEN("Result should be true") 
            {
                REQUIRE(ins_op_1_2.ExecuteOperation());
            }
        }

    }
}