#pragma once
#include <core/dcl_map.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <db/db_write.hpp>
#include <db/db_query_operations.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <functional>
#include <google/protobuf/util/time_util.h>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

namespace db_handler 
{
    class DBLocationHandler 
    {
    private:
        PBLocation *m_location;
        static const bsoncxx::document::value s_start_post;
        static const bsoncxx::document::value s_start_zone_access;
        static const bsoncxx::document::value s_start_dialog;
        static const std::unordered_map<std::string,void (db_handler::DBLocationHandler::*)(PBInteraction *,bsoncxx::document::view)> s_location_decoder;

        bsoncxx::document::value PostToBson(const PBPostInteraction &post_info) const;

        void BsonToPost(PBInteraction *interaction, bsoncxx::document::view doc);

        bsoncxx::document::value ZoneAccessToBson(const PBZoneAccessInteraction &zone_access_info) const;

        void BsonToZoneAccess(PBInteraction *interaction, bsoncxx::document::view doc);

        bsoncxx::document::value DialogToBson(const PBDialogInteraction &dialog_info) const;

        void BsonToDialog(PBInteraction *interaction, bsoncxx::document::view doc);
    
    public:
        DBLocationHandler(PBLocation *location) 
        {
            m_location = location;
        }

        /**
         * TESTED
         * Insert a new Location, the PBLocation in here is always 
         * a representation of the main DCL data with the interaction types
         * that need to be saved to the DataBase 
        */
        bool InsertNewLocation(PBUser &user, db::Transaction *t = nullptr);

        /**
         * TESTED
         * Find a player with the user id and player id
         * Inserts the types automatically
        */
        bool FindPlayerCurrentLocation(PBUser &user);
        
        bool ImprovePost(PBUser &user, int32_t interaction_id, PBUpgradeType type)
        {
            std::string upgrade_name;
            switch (type)
            {
            case PBUpgradeType::CAPACITY:
                upgrade_name = "capacity_upgrade";
                break;
            case PBUpgradeType::GEN_SECOND:
                upgrade_name = "gen_second_upgrade";
                break;
            case PBUpgradeType::FORTUNE:
                upgrade_name = "fortune_upgrade";
                break;
            
            default:
                break;
            }
            std::string array_update_query = "locations."+std::to_string(user.players(0).current_location())+"." + std::to_string(interaction_id) + ".post_info."+ upgrade_name;
            db::UpdateOneOperation update_op{"game_state",
                make_document(
                    kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                    kvp("player_id",b_int32{user.current_player_id()})
                ),
                make_document(kvp("$inc",make_document(
                    kvp(array_update_query, 1)
                )))
            };

            update_op.ExecuteOperation();

            return update_op.GetState() == db::OperationState::SUCCESS;
        }

        bool UnlockLocation(PBUser &user, int32_t interaction_id)
        {
            std::string array_update_query = "locations."+std::to_string(user.players(0).current_location())+"." + std::to_string(interaction_id) + ".zone_access_info.unlock_level";
            db::UpdateOneOperation update_op{"game_state",
                make_document(
                    kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                    kvp("player_id",b_int32{user.current_player_id()})
                ),
                make_document(kvp("$inc",make_document(
                    kvp(array_update_query, 1)
                )))
            };

            update_op.ExecuteOperation();

            return update_op.GetState() == db::OperationState::SUCCESS;
        }
    };
}