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
    public:
        DBLocationHandler(PBLocation *location) 
        {

        }

        static bool InsertNewLocation(PBUser &user, const DCLData::DCLLocation *location, db::Transaction *t = nullptr) 
        {
            if (!location->NeedsDatabase()) return false;

            bsoncxx::builder::basic::array array{};
            for (auto const &interaction : location->GetLocation()->interactions())
            {
                if (interaction.database_id() == -1) continue;

                bsoncxx::builder::basic::document doc{};
                for (auto const &type : interaction.types()) 
                {
                    switch (type)
                    {
                        case PBInteractionType::POST:
                            if(!interaction.post_info().needs_database()) continue;
                            doc.append(kvp("post_info",s_start_post.view()));
                            break;
                        case PBInteractionType::ZONE_ACCESS:
                            if(!interaction.zone_access_info().needs_database()) continue;
                            doc.append(kvp("zone_access_info",s_start_zone_access.view()));
                            break;
                        case PBInteractionType::DIALOG:
                            if(!interaction.dialog_info().needs_database()) continue;
                            doc.append(kvp("dialog_info",s_start_dialog.view()));
                            break;
                        default:
                            break;
                    }
                }
                array.append(doc.extract());
            }

            if (location->GetLocation()->loc_id() == PBLocationID::MAIN_BASE) 
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
                insert_op.ExecuteOperation();
                return insert_op.GetState() == db::OperationState::SUCCESS;
            } else 
            {
                db::UpdateOneOperation update_op{"game_state",
                    make_document(
                        kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
                        kvp("player_id",b_int32{user.current_player_id()})
                    ),
                    make_document(kvp("$set",make_document(
                        kvp("locations." + std::to_string(location->GetLocation()->database_id()),b_array{array})
                    )))
                };
                update_op.ExecuteOperation();
                return update_op.GetState() == db::OperationState::SUCCESS;
            }
        }
    };
}