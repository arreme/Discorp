#include <db_handler/db_handler.hpp>

using namespace bsoncxx::builder::basic;
using namespace bsoncxx::types;

bool db_handler::ChangeActivePlayer(uint64_t discord_id, int32_t newPlayerSlot) 
{
    db::UpdateOneOperation update_op{"users",
        make_document(kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)})),
        make_document(kvp("$set",make_document(
            kvp("current_player",b_int32{newPlayerSlot})
        )))
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::RegisterPlayerToDatabase(User &user, Player &player, std::vector<std::reference_wrapper<InteractionInfo>> &info) 
{
    bsoncxx::builder::basic::document insert{};
    auto doc = player.ToJson();
    for (auto elem : doc)
    {
        insert.append(kvp(elem.key(),elem.get_value()));
    }
    bsoncxx::builder::basic::array locations{};

    locations.append(FillInteracionsDocument(info));
    insert.append(kvp("locations",locations.extract()));

    db::Transaction t{};
    db::InsertOneOperation insert_op{"players",insert.extract()};
    t.AddOperation(&insert_op);
    db::InsertOneOperation insert_op_usrs{"users",user.ToJson()};
    t.AddOperation(&insert_op_usrs);
    
    t.ExecuteTransaction();

    return t.GetState() == db::OperationState::SUCCESS;
}

std::optional<User> db_handler::FindUserById(uint64_t discord_id) 
{
    db::FindOneOperation find_op{"users",make_document(kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}))};
    find_op.ExecuteOperation();
    if (find_op.m_result) 
    {
        return User{find_op.m_result.value().view()};
    }

    return std::nullopt;
}

bool db_handler::GoToLocation(Player &player, int32_t new_location) 
{
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$set",make_document(
            kvp("current_loc",b_int32{new_location})
        )))
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::FillLocation(Player &player, int32_t unlocked_location, std::vector<std::reference_wrapper<InteractionInfo>> &info)
{
    std::string location_update_query = "locations." + std::to_string(unlocked_location);
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$set",make_document(
            kvp(location_update_query,FillInteracionsDocument(info))
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::CollectPost(Player &player,  int32_t interaction_id)
{
    std::string array_update_query = "locations." + std::to_string(player.GetLocation()) + "." + std::to_string(interaction_id) + ".last_updated";
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$set",make_document(
            kvp(array_update_query, b_date{std::chrono::system_clock::now()})
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::ImprovePost(Player &player, int32_t interaction_id, std::string_view update_name)
{
    std::string array_update_query = "locations." + std::to_string(player.GetLocation()) + "." + std::to_string(interaction_id) + "."+update_name.data();
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$set",make_document(
            kvp(array_update_query, 1)
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
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

int db_handler::CurrentPlayerLocation(uint64_t discord_id, int32_t player_id) 
{
    mongocxx::options::find find_opt{};
    find_opt.projection(
        make_document(
            kvp("current_loc",1)
        )
    );
    db::FindOneOperation find_one_op{"players", 
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id",b_int32{player_id})
        ),
        std::move(find_opt)
    };

    find_one_op.ExecuteOperation();
    if (find_one_op.m_result) 
    {
        return find_one_op.m_result.value()["current_loc"].get_int32();
    }

    return -1;
}

bool db_handler::PlayerFirstTimeToLocation(Player &player,int32_t location_id) 
{
    mongocxx::options::find find_opt{};
    find_opt.projection(
        make_document(
            kvp("_id",1)
        )
    );
    db::FindOneOperation find_one_op{"players", 
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()}),
            kvp("locations."+std::to_string(location_id),
                make_document(
                    kvp("$type","array")
                )
            )
        ),
        std::move(find_opt)
    };

    find_one_op.ExecuteOperation();

    return !find_one_op.m_result.has_value();
};

bsoncxx::array::value db_handler::FillInteracionsDocument(std::vector<std::reference_wrapper<InteractionInfo>> &info) 
{
    bsoncxx::builder::basic::array interactions{};

    for (auto curr : info)
    {
        interactions.append(curr.get().ToJson());
    }

    return interactions.extract();
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

