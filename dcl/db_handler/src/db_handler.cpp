#include <db_handler/db_handler.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

bool db_handler::RegisterPlayerToDatabase(Player &player, std::vector<InteractionInfo *> &info) 
{
    bsoncxx::builder::basic::document insert{};
    auto doc = player.ToJson();
    for (auto elem : doc)
    {
        insert.append(kvp(elem.key(),elem.get_value()));
    }
    bsoncxx::builder::basic::array locations{};
    bsoncxx::builder::basic::array current_loc{};
    for (auto elem : info)
    {
        current_loc.append(elem->ToJson());
    }
    locations.append(current_loc.extract());
    insert.append(kvp("locations",locations.extract()));

    db::InsertOneOperation insert_op{"players",insert.extract()};
    insert_op.ExecuteOperation();
    return insert_op.GetState() == db::OperationState::SUCCESS;
}

std::optional<db_handler::playerInteractions> db_handler::FindPlayerCurrentLocationInteractions(uint64_t discord_id, int32_t player_id) 
{
    db::AggregateOperation agg_op{"players",PlayerCurrentLocationInteractions_Pipeline(discord_id,player_id)};
    agg_op.ExecuteOperation();
    if(agg_op.GetState() == db::OperationState::SUCCESS) 
    {
        auto result = agg_op.GetResult();
        if (result) 
        {
            auto doc = result.value();
            return db_handler::playerInteractions{doc,InteractionInfo::CreateInteractions(doc)};
        }   
    }
    return std::nullopt;
}

std::optional<std::pair<Player,std::unique_ptr<InteractionInfo>>> db_handler::FindPlayerCurrentInteraction(uint64_t discord_id, int32_t player_id, int32_t interaction_id)
{
    db::AggregateOperation agg_op{"players",PlayerCurrentInteraction_Pipeline(discord_id,player_id,interaction_id)};
    agg_op.ExecuteOperation();
    if(agg_op.GetState() == db::OperationState::SUCCESS) 
    {
        auto result = agg_op.GetResult();
        if (result) 
        {
            auto doc = result.value();
            return std::pair<Player,std::unique_ptr<InteractionInfo>>{doc,InteractionInfo::CreateInteraction(doc)};
        }   
    }
    return std::nullopt;
}

mongocxx::pipeline db_handler::PlayerCurrentLocationInteractions_Pipeline(uint64_t discord_id, int32_t player_id) 
{
    mongocxx::pipeline p{};
    p.match(make_document(
        kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
        kvp("player_id", b_int32{player_id})
    ));
    p.add_fields(make_document(
        kvp("interactions", make_document(
            kvp("$arrayElemAt", make_array(
                "$locations",
                "$current_loc"
            ))
        ))
    ));
    p.append_stage(make_document(
        kvp("$unset","locations")
    ));
    return p;
}

mongocxx::pipeline db_handler::PlayerCurrentInteraction_Pipeline(uint64_t discord_id, int32_t player_id, int32_t interaction_id) 
{
    mongocxx::pipeline p{};
    p.match(make_document(
        kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
        kvp("player_id", b_int32{player_id})
    ));
    p.add_fields(make_document(
        kvp("interactions", make_document(
            kvp("$arrayElemAt", make_array(
                make_document(
                    kvp("$arrayElemAt", make_array(
                        "$locations",
                        "$current_loc"
                    ))
                ),
                b_int32{interaction_id}
            ))
        ))
    ));
    p.append_stage(make_document(
        kvp("$unset","locations")
    ));
    return p;
}

