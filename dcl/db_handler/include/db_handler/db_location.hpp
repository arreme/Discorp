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

        bool InsertNewLocation(PBUser &user, db::Transaction *t = nullptr) 
        {
            if (m_location->database_id() == -1) return false;

            bsoncxx::builder::basic::array array{};
            for (auto const &interaction : m_location->interactions())
            {
                if (interaction.database_id() == -1) continue;

                bsoncxx::builder::basic::document doc{};
                for (auto const &type : interaction.types()) 
                {
                    switch (type)
                    {
                        case PBInteractionType::POST:
                            doc.append(kvp("post_info",b_document{PostToBson(interaction.post_info())}));
                            break;
                        case PBInteractionType::ZONE_ACCESS:
                            doc.append(kvp("zone_access_info",ZoneAccessToBson(interaction.zone_access_info())));
                            break;
                        case PBInteractionType::DIALOG:
                            doc.append(kvp("dialog_info",DialogToBson(interaction.dialog_info())));
                            break;
                        default:
                            break;
                    }
                }
                array.append(doc.extract());
            }

            if (m_location->id() == PBLocationID::MAIN_BASE) 
            {
                //First Time, we need to build the entire document
                bsoncxx::builder::basic::array init_array{};
                init_array.append(array);
                db::InsertOneOperation insert_op{"game_state",
                    make_document(
                        kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                        kvp("player_id",b_int32{user.current_player_id()}),
                        kvp("locations",init_array)
                    )
                };
                if (t) 
                {
                    t->AddOperation(std::make_unique<db::InsertOneOperation>(insert_op));
                    return true;
                } else {
                    insert_op.ExecuteOperation();
                    return insert_op.GetState() == db::OperationState::SUCCESS;
                }
                
            } else 
            {
                db::UpdateOneOperation update_op{"game_state",
                    make_document(
                        kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                        kvp("player_id",b_int32{user.current_player_id()})
                    ),
                    make_document(kvp("$set",make_document(
                        kvp("locations." + std::to_string(m_location->database_id()),b_array{array})
                    )))
                };
                if (t) 
                {
                    t->AddOperation(std::make_unique<db::UpdateOneOperation>(update_op));
                    return true;
                } else {
                    update_op.ExecuteOperation();
                    return update_op.GetState() == db::OperationState::SUCCESS;
                }
                
            }
        }

        bool FindPlayerCurrentLocation(PBUser &user) 
        {
            return false;
        }
    };
}