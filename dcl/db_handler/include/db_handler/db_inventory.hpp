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
        std::vector<PBItemData> *items;
    public:
        DBInventoryHandler() 
        {

        }

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
    };
}