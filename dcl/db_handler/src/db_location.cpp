#include <db_handler/db_location.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

bsoncxx::document::value db_handler::DBLocationHandler::s_start_post = 
    make_document(
        kvp("capacity_upgrade",b_int32{0}),
        kvp("gen_second_upgrade",b_int32{0}),
        kvp("fortune_upgrade",b_int32{0}),
        kvp("resource_stored",b_int64{0})  
    );

bsoncxx::document::value db_handler::DBLocationHandler::s_start_zone_access =
    make_document(
        kvp("unlock_level",b_int32{0}) 
    );

bsoncxx::document::value db_handler::DBLocationHandler::s_start_dialog =
    make_document(
        kvp("current_dialog",b_int32{0}) 
    );

bool db_handler::DBLocationHandler::InsertNewLocation(PBUser &user, db::Transaction *t) 
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