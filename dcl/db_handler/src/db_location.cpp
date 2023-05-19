#include <db_handler/db_location.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

const bsoncxx::document::value db_handler::DBLocationHandler::s_start_post = 
    make_document(
        kvp("capacity_upgrade",b_int32{0}),
        kvp("gen_second_upgrade",b_int32{0}),
        kvp("fortune_upgrade",b_int32{0}),
        kvp("resource_stored",b_int64{0})  
    );

const bsoncxx::document::value db_handler::DBLocationHandler::s_start_zone_access =
    make_document(
        kvp("unlock_level",b_int32{0}) 
    );

const bsoncxx::document::value db_handler::DBLocationHandler::s_start_dialog =
    make_document(
        kvp("current_dialog",b_int32{0}) 
    );

const std::unordered_map<std::string,void (db_handler::DBLocationHandler::*)(PBInteraction *,bsoncxx::document::view)> db_handler::DBLocationHandler::s_location_decoder = 
    {{"post_info",&db_handler::DBLocationHandler::BsonToPost},
     {"dialog_info",&db_handler::DBLocationHandler::BsonToDialog},
     {"zone_access_info",&db_handler::DBLocationHandler::BsonToZoneAccess}};


bsoncxx::document::value db_handler::DBLocationHandler::PostToBson(const PBPostInteraction &post_info) const
{
    return make_document(
                kvp("capacity_upgrade",b_int32{post_info.capacity_upgrade()}),
                kvp("gen_second_upgrade",b_int32{post_info.gen_second_upgrade()}),
                kvp("fortune_upgrade",b_int32{post_info.fortune_upgrade()}),
                kvp("resource_stored",b_int32{post_info.resource_stored()}),
                kvp("last_collected",b_date{std::chrono::seconds(google::protobuf::util::TimeUtil::TimestampToSeconds(post_info.last_collected()))})  
            );
}

void db_handler::DBLocationHandler::BsonToPost(PBInteraction *interaction, bsoncxx::document::view doc) 
{
    auto post_info = interaction->mutable_post_info();
    interaction->add_types(PBInteractionType::POST);
    post_info->set_capacity_upgrade(doc["capacity_upgrade"].get_int32());
    post_info->set_gen_second_upgrade(doc["gen_second_upgrade"].get_int32());
    post_info->set_fortune_upgrade(doc["fortune_upgrade"].get_int32());
    post_info->set_resource_stored(doc["resource_stored"].get_int32());
    std::chrono::system_clock::time_point time = doc["last_collected"].get_date();
    post_info->mutable_last_collected()->set_seconds(std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count());
}

bsoncxx::document::value db_handler::DBLocationHandler::ZoneAccessToBson(const PBZoneAccessInteraction &zone_access_info) const
{
    return make_document(
                kvp("unlock_level",b_int32{zone_access_info.unlock_level()}) 
            );
}

void db_handler::DBLocationHandler::BsonToZoneAccess(PBInteraction *interaction, bsoncxx::document::view doc) 
{
    interaction->add_types(PBInteractionType::ZONE_ACCESS);
    auto zone_access_info = interaction->mutable_zone_access_info();
    zone_access_info->set_unlock_level(doc["unlock_level"].get_int32());
}

bsoncxx::document::value db_handler::DBLocationHandler::DialogToBson(const PBDialogInteraction &dialog_info) const
{
    return make_document(
                kvp("current_dialog",b_int32{dialog_info.current_dialog()}) 
            );
}

void db_handler::DBLocationHandler::BsonToDialog(PBInteraction *interaction, bsoncxx::document::view doc) 
{
    interaction->add_types(PBInteractionType::DIALOG);
    auto dialog_info = interaction->mutable_dialog_info();
    dialog_info->set_current_dialog(doc["current_dialog"].get_int32());
}


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


bool db_handler::DBLocationHandler::FindPlayerCurrentLocation(PBUser &user) 
{
    mongocxx::pipeline p;
    p.match(make_document(
        kvp("discord_id", b_int64{static_cast<int64_t>(user.discord_id())}),
        kvp("player_id", b_int32{user.current_player_id()})
    ));
    p.add_fields(make_document(
        kvp("locations", make_document(
            kvp("$arrayElemAt", make_array(
                "$locations",
                b_int32{DCLData::DCLMap::getInstance().GetLocation(user.players(0).current_location())->GetLocationDBID()}
            ))
        ))
    ));

    db::AggregateOperation agg_op{"game_state",std::move(p)};
    agg_op.ExecuteOperation();
    m_location->clear_interactions();
    if (agg_op.GetState() == db::OperationState::SUCCESS) 
    {
        auto result = agg_op.GetResult();
        if (result.has_value()) 
        {
            auto doc = result.value();
            for (auto const &interaction_element : doc["locations"].get_array().value)
            {
                
                auto interaction = interaction_element.get_document().value;
                auto temp_interaction = m_location->add_interactions();
                for (auto const &element : interaction)
                {
                    (this->*s_location_decoder.at(std::string{element.key()}))(temp_interaction,interaction[element.key()].get_document().value);
                }
            }
            return true;
        }
    }
    return false;
}