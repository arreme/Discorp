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

        bsoncxx::document::value PostToBson(const PBPostInteraction &post_info) const
        {
            return make_document(
                        kvp("capacity_upgrade",b_int32{post_info.capacity_upgrade()}),
                        kvp("gen_second_upgrade",b_int32{post_info.gen_second_upgrade()}),
                        kvp("fortune_upgrade",b_int32{post_info.fortune_upgrade()}),
                        kvp("resource_stored",b_int64{post_info.resource_stored()})  
                    );
        }

        void BsonToPost(PBInteraction *interaction, bsoncxx::document::view doc) 
        {
            auto post_info = interaction->mutable_post_info();
            interaction->add_types(PBInteractionType::POST);
            post_info->set_capacity_upgrade(doc["capacity_upgrade"].get_int32());
            post_info->set_gen_second_upgrade(doc["gen_second_upgrade"].get_int32());
            post_info->set_fortune_upgrade(doc["fortune_upgrade"].get_int32());
            post_info->set_resource_stored(doc["resource_stored"].get_int64());
        }

        bsoncxx::document::value ZoneAccessToBson(const PBZoneAccessInteraction &zone_access_info) const
        {
            return make_document(
                        kvp("unlock_level",b_int32{zone_access_info.unlock_level()}) 
                    );
        }

        void BsonToZoneAccess(PBInteraction *interaction, bsoncxx::document::view doc) 
        {
            interaction->add_types(PBInteractionType::ZONE_ACCESS);
            auto zone_access_info = interaction->mutable_zone_access_info();
            zone_access_info->set_unlock_level(doc["unlock_level"].get_int32());
        }

        bsoncxx::document::value DialogToBson(const PBDialogInteraction &dialog_info) const
        {
            return make_document(
                        kvp("current_dialog",b_int32{dialog_info.current_dialog()}) 
                    );
        }

        void BsonToDialog(PBInteraction *interaction, bsoncxx::document::view doc) 
        {
            interaction->add_types(PBInteractionType::DIALOG);
            auto dialog_info = interaction->mutable_dialog_info();
            dialog_info->set_current_dialog(doc["current_dialog"].get_int32());
        }
    
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
        
    };
}