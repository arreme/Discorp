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
        static bool InitializeInventory(PBUser &user, db::Transaction *t = nullptr);

        bool UpdateItems(uint64_t discord_id, int32_t player_id);

        bool ModifyItemsQuantity(uint64_t discord_id, int32_t player_id);
        
        bool GetItems(uint64_t discord_id, int32_t player_id, std::string category);

        bool GetInventory(uint64_t discord_id, int32_t player_id, std::string category);
    };
}