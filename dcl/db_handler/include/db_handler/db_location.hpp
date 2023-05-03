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

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

namespace db_handler 
{
    class DBLocationHandler 
    {
    private:
        PBLocation *m_location;
        static bsoncxx::document::value s_start_post;
        static bsoncxx::document::value s_start_zone_access;
        static bsoncxx::document::value s_start_dialog;

        bsoncxx::document::value PostToBson(const PBPostInteraction &post_info) const
        {
            return make_document(
                        kvp("capacity_upgrade",b_int32{post_info.capacity_upgrade()}),
                        kvp("gen_second_upgrade",b_int32{post_info.gen_second_upgrade()}),
                        kvp("fortune_upgrade",b_int32{post_info.fortune_upgrade()}),
                        kvp("resource_stored",b_int64{post_info.resource_stored()})  
                    );
        }

        void BsonToPost(PBPostInteraction *post_info, bsoncxx::document::view doc) 
        {

        }

        bsoncxx::document::value ZoneAccessToBson(const PBZoneAccessInteraction &zone_access_info) const
        {
            return make_document(
                        kvp("unlock_level",b_int32{zone_access_info.unlock_level()}) 
                    );
        }

        void BsonToZoneAccess(PBZoneAccessInteraction *zone_access_info, bsoncxx::document::view doc) 
        {

        }

        bsoncxx::document::value DialogToBson(const PBDialogInteraction &dialog_info) const
        {
            return make_document(
                        kvp("current_dialog",b_int32{dialog_info.current_dialog()}) 
                    );
        }

        void BsonToDialog(PBDialogInteraction *dialog_info, bsoncxx::document::view doc) 
        {

        }
    public:
        DBLocationHandler(PBLocation *location) 
        {
            m_location = location;
        }

        /*
        * TESTED
        * Insert a new Location, the PBLocation in here is always 
        * a representation of the main DCL data with the interaction types
        * that need to be saved to the DataBase
        */
        bool InsertNewLocation(PBUser &user, db::Transaction *t = nullptr);

        bool FindPlayerCurrentLocation(PBUser &user) 
        {
            
            return false;
        }
    };
}