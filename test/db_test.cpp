#include <catch2/catch_test_macros.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <db/db_operation.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>

using bsoncxx::builder::basic::kvp;
using namespace bsoncxx::types;

SCENARIO("Testing database basic operations","[db]") 
{
    db::DeleteManyOperation del_op = db::DeleteManyOperation("test_col", bsoncxx::builder::basic::make_document());
    GIVEN("1.- A new empty user collection collection") 
    {
        del_op.ExecuteOperation();
        
        WHEN("1.1.- FindOne Operation") 
        {
            db::FindOneOperation find_op_1_1{"users",bsoncxx::builder::basic::make_document(kvp("discord_id",0))};
            find_op_1_1.ExecuteOperation();
            THEN("Result should be null") 
            {
                REQUIRE_FALSE(find_op_1_1.m_result);
            }
        }
        WHEN("1.2.- Adding document to the collection") 
        {
            uint64_t id = 0;
            auto doc = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)}),
                kvp("name",b_string{"John Doe"})
            );

            db::InsertOneOperation ins_op_1_2{"test_col", std::move(doc)};
            
            THEN("Result should be SUCCESS") 
            {
                ins_op_1_2.ExecuteOperation();
                REQUIRE(ins_op_1_2.GetState() == db::OperationState::SUCCESS);
            }
        }
        WHEN("1.3.- Deleting a document") 
        {
            uint64_t id = 0;
            auto doc = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)})
            );

            db::DeleteOneOperation del_op_1_3{"test_col", std::move(doc)};
            
            THEN("Result should be SUCCESS") 
            {
                del_op_1_3.ExecuteOperation();
                REQUIRE(del_op_1_3.GetState() == db::OperationState::SUCCESS);
            }
        }
        WHEN("1.4.- Making a transaction") 
        {
            uint64_t id1 = 0;
            auto doc1 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id1)}),
                kvp("name",b_string{"John One"})
            );

            uint64_t id2 = 1;
            auto doc2 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id2)}),
                kvp("name",b_string{"John Doe"})
            );

            uint64_t id3 = 2;
            auto doc3 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id3)}),
                kvp("name",b_string{"John Thre"})
            );

            db::InsertOneOperation ins1_op_3{"test_col", std::move(doc1)};
            db::InsertOneOperation ins2_op_3{"test_col", std::move(doc2)};
            db::InsertOneOperation ins3_op_3{"test_col", std::move(doc3)};

            db::Transaction t_op{};
            t_op.AddOperation(&ins1_op_3);
            t_op.AddOperation(&ins2_op_3);
            t_op.AddOperation(&ins3_op_3);

            t_op.ExecuteTransaction();

            REQUIRE(t_op.GetState() == db::OperationState::SUCCESS);
            REQUIRE(ins1_op_3.GetState() == db::OperationState::SUCCESS);
            REQUIRE(ins2_op_3.GetState() == db::OperationState::SUCCESS);
            REQUIRE(ins3_op_3.GetState() == db::OperationState::SUCCESS);
        }

        /*CLEAN UP*/
        del_op.ExecuteOperation();
    }

    GIVEN("2.- A collection with a document") 
    {
        uint64_t id = 0;
        auto doc = bsoncxx::builder::basic::make_document(
            kvp("id",b_int64{static_cast<int64_t>(id)}),
            kvp("name",b_string{"John Doe"})
        );

        db::InsertOneOperation ins_op_2{"test_col", std::move(doc)};

        ins_op_2.ExecuteOperation();
        REQUIRE(ins_op_2.GetState() == db::OperationState::SUCCESS);
        WHEN("2.1.- Inserting with the same id") 
        {
            ins_op_2.ExecuteOperation();
            THEN("Result should be duplicated id") 
            {
                REQUIRE(ins_op_2.GetState() == db::OperationState::DUPLICATED_ID);
            }
        }
        WHEN("2.2.- Looking for a document") 
        {

            auto doc_find_2_2 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)})
            );
            db::FindOneOperation find_op_2_2{"test_col",std::move(doc_find_2_2)};

            THEN("Result should return document") 
            {
                find_op_2_2.ExecuteOperation();
                REQUIRE(find_op_2_2.m_result);
                auto res_2_2 = find_op_2_2.m_result.value().view();
                REQUIRE(res_2_2["name"].get_string().value == "John Doe");
            }
        }
        WHEN("2.3.- Deleting an existing document") 
        {
            auto doc_2_3 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)})
            );

            db::DeleteOneOperation del_op_2_3{"test_col",std::move(doc_2_3)};
            THEN("Result should be a success") {
                del_op_2_3.ExecuteOperation();
                REQUIRE(del_op_2_3.GetState() == db::OperationState::SUCCESS);
                auto find_doc_2_3 = bsoncxx::builder::basic::make_document(
                    kvp("id",b_int64{static_cast<int64_t>(id)})
                );
                db::FindOneOperation find_op_2_3{"test_col",std::move(find_doc_2_3)};
                find_op_2_3.ExecuteOperation();
                REQUIRE_FALSE(find_op_2_3.m_result);
            }
        }

        /*CLEAN UP*/
        del_op.ExecuteOperation();
    }

    GIVEN("3.- A collection with multiple documents") 
    {
        uint64_t id1 = 0;
        auto doc1 = bsoncxx::builder::basic::make_document(
            kvp("id",b_int64{static_cast<int64_t>(id1)}),
            kvp("name",b_string{"John Doe"})
        );

        uint64_t id2 = 1;
        auto doc2 = bsoncxx::builder::basic::make_document(
            kvp("id",b_int64{static_cast<int64_t>(id2)}),
            kvp("name",b_string{"John Doe"})
        );

        uint64_t id3 = 2;
        auto doc3 = bsoncxx::builder::basic::make_document(
            kvp("id",b_int64{static_cast<int64_t>(id3)}),
            kvp("name",b_string{"John Doe"})
        );

        db::InsertOneOperation ins1_op_3{"test_col", std::move(doc1)};
        db::InsertOneOperation ins2_op_3{"test_col", std::move(doc2)};
        ins1_op_3.ExecuteOperation();
        ins2_op_3.ExecuteOperation();

        WHEN("3.1.- Deleting all") 
        {

            del_op.ExecuteOperation();
            uint64_t id = 0;
            auto doc_find_3_1 = bsoncxx::builder::basic::make_document(
                kvp("id",b_int64{static_cast<int64_t>(id)})
            );
            db::FindOneOperation find_op_3_1{"test_col",std::move(doc_find_3_1)};

            REQUIRE_FALSE(find_op_3_1.m_result);
        }
    }
}