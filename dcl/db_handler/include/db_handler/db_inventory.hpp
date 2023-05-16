#pragma once
#include <core/pb/player.pb.h>
#include <core/pb/item.pb.h>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/string/to_string.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

namespace db_handler 
{
    class DBInventoryHandler 
    {
    private:
        std::vector<PBItemData> *m_items;
    public:
        DBInventoryHandler(std::vector<PBItemData> *items) 
        : m_items(items)
        {

        }
        /**
         * TESTED
         * Inserts an empty inventory with all the required tabs to the database
        */
        static bool InitializeInventory(PBUser &user, db::Transaction *t = nullptr) 
        {
            
            db::InsertOneOperation insert_inventory{"inventory",
                make_document(
                    kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                    kvp("player_id",b_int32{user.current_player_id()}),
                    kvp(PBItemType_Name(PBItemType::RESOURCES),b_array{}),
                    kvp(PBItemType_Name(PBItemType::ATTACKS),b_array{}),
                    kvp(PBItemType_Name(PBItemType::WEAPONS),b_array{}),
                    kvp(PBItemType_Name(PBItemType::EQUIPMENT),b_array{}),
                    kvp(PBItemType_Name(PBItemType::QUEST_ITEMS),b_array{})
                )
            };

            if (t) 
            {
                t->AddOperation(std::make_unique<db::InsertOneOperation>(insert_inventory));
                return true;
            } else {
                insert_inventory.ExecuteOperation();
                return insert_inventory.GetState() == db::OperationState::SUCCESS;
            }
        }

        bool UpdateItems(uint64_t discord_id, int32_t player_id) 
        {

            bsoncxx::builder::basic::document set_doc{};
            bsoncxx::builder::basic::document inc_doc{};

            for (auto const &item : *m_items)
            {
                int type = item.item_id() / 1000;
                set_doc.append(kvp(PBItemType_Name(type)+"."+std::to_string(item.item_id())+".item_id",b_int32{item.item_id()}));
                inc_doc.append(kvp(PBItemType_Name(type)+"."+std::to_string(item.item_id())+".quantity",b_int32{item.subtract() ? -item.quantity() : item.quantity()}));
            }
            
            db::UpdateOneOperation update_op{"inventory",
                make_document(
                    kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
                    kvp("player_id", b_int32{player_id})
                ),
                make_document(
                    kvp("$set", set_doc.extract()),
                    kvp("$set", inc_doc.extract())
                )
            };

            update_op.ExecuteOperation();
            return update_op.GetState() == db::OperationState::SUCCESS;
        }

        bool GetItems(uint64_t discord_id, int32_t player_id, std::string category) 
        {
            bsoncxx::builder::basic::array ids{};

            for (auto const &item_id : *m_items)
            {
                ids.append(item_id.item_id());
            }
            
            
            mongocxx::options::find find_options{};
            find_options.projection(
                make_document(
                    kvp(category,make_document(
                        kvp("$filter",make_document(
                            kvp("input","$"+category),
                            kvp("as","item"),
                            kvp("cond",make_document(
                                kvp("$in",make_array(
                                    "$$item.item_id",
                                    ids.extract()
                                ))
                            ))
                        ))
                    ))
                )
            );

            db::FindOneOperation find_op{"inventory",
                make_document(
                    kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
                    kvp("player_id", b_int32{player_id})
                ),
                std::move(find_options)
            };
            find_op.ExecuteOperation();
            m_items->clear();
            if (find_op.m_result) 
            {
                auto item_array = find_op.m_result.value()[category].get_array().value;
                
                for (auto const &current : item_array)
                {
                    PBItemData temp;
                    temp.set_item_id(static_cast<PBItemEnum>(static_cast<int32_t>(current["item_id"].get_int32())));
                    temp.set_quantity(current["quantity"].get_int32());
                    m_items->push_back(temp);
                }
                return true;  
            }
            return false;
        }

        bool GetInventory(uint64_t discord_id, int32_t player_id, std::string category)
        {
            mongocxx::options::find find_options{};
            find_options.projection(
                make_document(
                    kvp(category,1)
                )
            );
            db::FindOneOperation find_one_op("inventory",
                make_document(
                    kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
                    kvp("player_id", b_int32{player_id})
                ),
                std::move(find_options)
            );

            find_one_op.ExecuteOperation();
            m_items->clear();
            if (find_one_op.m_result) 
            {
                auto item_array = find_one_op.m_result.value()[category].get_array().value;
                for (auto& item : item_array)
                {
                    /* code */
                    if (item.type() != bsoncxx::type::k_null) 
                    {
                        PBItemData temp;
                        temp.set_item_id(static_cast<PBItemEnum>(static_cast<int32_t>(item["item_id"].get_int32())));
                        temp.set_quantity(item["quantity"].get_int32());
                        m_items->push_back(temp);
                    }
                }
                return true;
                
            }
            return false;
        }
    };
}