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
    db::InsertOneOperation insert_inventory{"inventory",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(user.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()}),
            kvp(Item::RESOURCE_TYPE,b_array{}),
            kvp(Item::UTILITIES_TYPE,b_array{}),
            kvp(Item::WEAPONS_TYPE,b_array{}),
            kvp(Item::EQUIPMENT_TYPE,b_array{}),
            kvp(Item::QUEST_ITEMS_TYPE,b_array{})
        )
    };
    t.AddOperation(&insert_inventory);
    
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

bool db_handler::GoToLocation(uint64_t discord_id, int32_t player_id, int32_t new_location) 
{
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id",b_int32{player_id})
        ),
        make_document(kvp("$set",make_document(
            kvp("current_loc",b_int32{new_location})
        )))
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::UnlockLocation(Player &player, int32_t location_id, int32_t interaction) 
{
    std::string location_update_query = "locations."+std::to_string(location_id)+"." +std::to_string(interaction)+".ZONE_UNLOCK";
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$inc",make_document(
            kvp(location_update_query,1)
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::FillLocation(uint64_t discord_id, int32_t player_id, int32_t unlocked_location, std::vector<std::reference_wrapper<InteractionInfo>> &info)
{
    std::string location_update_query = "locations." + std::to_string(unlocked_location);
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id",b_int32{player_id})
        ),
        make_document(kvp("$set",make_document(
            kvp(location_update_query,FillInteracionsDocument(info))
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::CollectPost(Player &player,  int32_t interaction_id, int32_t resource_stored, std::string category, std::vector<Item> &item_modifiers, bool update_time)
{
    db::Transaction t{};

    std::string location_update_time = "locations." + std::to_string(player.GetLocation()) + "." + std::to_string(interaction_id) + ".last_updated";
    std::string location_update_resource = "locations." + std::to_string(player.GetLocation()) + "." + std::to_string(interaction_id) + ".resource_stored";
    db::UpdateOneOperation update_post = update_time ?
        db::UpdateOneOperation{"players",
            make_document(
                kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
                kvp("player_id",b_int32{player.GetPlayerId()})
            ),
            make_document(kvp("$set",make_document(
                kvp("stats",b_document{player.GetStats()->ToJson()}),
                kvp("skills",b_document{player.GetSkills()->ToJson()}),
                kvp(location_update_time, b_date{std::chrono::system_clock::now()}),
                kvp(location_update_resource, b_int32{resource_stored})
            )))
        } :
        db::UpdateOneOperation{"players",
            make_document(
                kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
                kvp("player_id",b_int32{player.GetPlayerId()})
            ),
            make_document(kvp("$set",make_document(
                kvp("stats",b_document{player.GetStats()->ToJson()}),
                kvp("skills",b_document{player.GetSkills()->ToJson()}),
                kvp(location_update_resource, b_int32{resource_stored})
            )))
        };
    

    bsoncxx::builder::basic::document set_doc{};
    bsoncxx::builder::basic::document inc_doc{};

    for (auto &item : item_modifiers)
    {
        set_doc.append(kvp(category+"."+std::to_string(item.GetItemId())+".item_id",b_int32{item.GetItemId()}));
        inc_doc.append(kvp(category+"."+std::to_string(item.GetItemId())+".quantity",b_int32{item.GetQuantity()}));
    }
    
    db::UpdateOneOperation update_items{"inventory",
        make_document(
            kvp("discord_id", b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id", b_int32{player.GetPlayerId()})
        ),
        make_document(
            kvp("$set", set_doc.extract()),
            kvp("$inc", inc_doc.extract())
        )
    };

    t.AddOperation(&update_post);
    t.AddOperation(&update_items);

    t.ExecuteTransaction();

    return t.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::ImprovePost(Player &player, int32_t interaction_id, std::string_view update_name)
{
    std::string array_update_query = "locations."+std::to_string(player.GetLocation())+"." + std::to_string(interaction_id) + "."+ update_name.data();
    db::UpdateOneOperation update_op{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$inc",make_document(
            kvp(array_update_query, 1)
        )))
    };

    update_op.ExecuteOperation();

    return update_op.GetState() == db::OperationState::SUCCESS;
}

std::optional<db_handler::playerInteractions> db_handler::FindPlayerCurrentLocationInteractions(uint64_t discord_id, int32_t player_id, int32_t location_db_id) 
{
    db::AggregateOperation agg_op{"players",PlayerCurrentLocationInteractions_Pipeline(discord_id,player_id, location_db_id)};
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

std::optional<std::pair<Player,std::unique_ptr<InteractionInfo>>> db_handler::FindPlayerCurrentInteraction(uint64_t discord_id, int32_t player_id, int32_t location_db_id, int32_t interaction_id)
{
    db::AggregateOperation agg_op{"players",PlayerCurrentInteraction_Pipeline(discord_id,player_id,location_db_id,interaction_id)};
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

bool db_handler::PlayerFirstTimeToLocation(uint64_t discord_id, int32_t player_id, int32_t location_id) 
{
    mongocxx::options::find find_opt{};
    find_opt.projection(
        make_document(
            kvp("_id",1)
        )
    );
    db::FindOneOperation find_one_op{"players", 
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id",b_int32{player_id}),
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

mongocxx::pipeline db_handler::PlayerCurrentLocationInteractions_Pipeline(uint64_t discord_id, int32_t player_id, int32_t location_db_id) 
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
                b_int32{location_db_id}
            ))
        ))
    ));
    p.append_stage(make_document(
        kvp("$unset","locations")
    ));
    return p;
}

mongocxx::pipeline db_handler::PlayerCurrentInteraction_Pipeline(uint64_t discord_id, int32_t player_id, int32_t location_db_id, int32_t interaction_id) 
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
                        b_int32{location_db_id}
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

//INVENTORY

bool db_handler::ModifyItemQuantity(uint64_t discord_id, int32_t player_id, std::string category, int item_id, int quantity) 
{

    db::UpdateOneOperation update_op{"inventory",
        make_document(
            kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        make_document(
            kvp("$set",make_document(
                kvp(category+"."+std::to_string(item_id)+".item_id",b_int32{item_id})
            )),
            kvp("$inc",make_document(
                kvp(category+"."+std::to_string(item_id)+".quantity",b_int32{quantity})
            ))
        )
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::ModifyItemsQuantity(uint64_t discord_id, int32_t player_id, std::string category, std::vector<Item> &item_modifiers, bool subtract) 
{

    bsoncxx::builder::basic::document set_doc{};
    bsoncxx::builder::basic::document inc_doc{};

    for (auto &item : item_modifiers)
    {
        set_doc.append(kvp(category+"."+std::to_string(item.GetItemId())+".item_id",b_int32{item.GetItemId()}));
        inc_doc.append(kvp(category+"."+std::to_string(item.GetItemId())+".quantity",b_int32{subtract ? -item.GetQuantity() : item.GetQuantity()}));
    }
    
    db::UpdateOneOperation update_op{"inventory",
        make_document(
            kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        make_document(
            kvp("$set", set_doc.extract()),
            kvp("$inc", inc_doc.extract())
        )
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

std::optional<Item> db_handler::GetItem(uint64_t discord_id, int32_t player_id, std::string category, int item_id) 
{

    mongocxx::options::find find_options{};
    find_options.projection(
        make_document(
            kvp(category,make_document(
                kvp("$slice",make_array(
                    item_id,
                    1
                ))
            ))
        )
    );

    db::FindOneOperation find_op{"inventory",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        std::move(find_options)
    };
    find_op.ExecuteOperation();
    if (find_op.m_result) 
    {
        auto item_array = find_op.m_result.value()["resources"].get_array().value;
        if (std::distance(item_array.begin(),item_array.end()) == 1)
            return Item{item_array[0].get_document()};
    }

    return std::nullopt;
}

std::vector<Item> db_handler::GetItems(uint64_t discord_id, int32_t player_id, std::string category, std::vector<Item> &item_ids) 
{
    bsoncxx::builder::basic::array ids{};

    for (auto &item_id : item_ids)
    {
        ids.append(item_id.GetItemId());
    }
    
    
    mongocxx::options::find find_options{};
    find_options.projection(
        make_document(
            kvp(category,make_document(
                kvp("$filter",make_document(
                    kvp("input","$"+category),
                    kvp("as","item"),
                    kvp("cond",make_document(
                        kvp("$in",make_array(
                            "$$item.item_id",
                            ids.extract()
                        ))
                    ))
                ))
            ))
        )
    );

    db::FindOneOperation find_op{"inventory",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        std::move(find_options)
    };
    find_op.ExecuteOperation();
    if (find_op.m_result) 
    {
        auto item_array = find_op.m_result.value()[category].get_array().value;
        
        if (std::distance(item_array.begin(),item_array.end()) >= 1) {
            std::vector<Item> item_data;
            for (auto &current : item_array)
            {
                item_data.push_back(Item{current.get_document().view()});
            }

            return item_data;
            
        }   
    }
    return {};
}

std::vector<Item> db_handler::GetInventory(uint64_t discord_id, int32_t player_id, std::string category)
{
    mongocxx::options::find find_options{};
    find_options.projection(
        make_document(
            kvp(category,1)
        )
    );
    db::FindOneOperation find_one_op("inventory",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        std::move(find_options)
    );

    find_one_op.ExecuteOperation();

    if (find_one_op.m_result) 
    {
        auto item_array = find_one_op.m_result.value()[category].get_array().value;
        std::vector<Item> item_data;
        for (auto& item : item_array)
        {
            /* code */
            if (item.type() != bsoncxx::type::k_null) 
            {
                item_data.push_back(Item{item.get_document().view()});
            }
        }
        return item_data;
        
    }
    return {};
}

bool db_handler_util::ModifyPostDate(Player &player, int32_t interaction_id, int32_t secondsToModify) 
{
    auto modifiedTime = std::chrono::system_clock::now();
    modifiedTime += std::chrono::seconds(secondsToModify);
    std::string location_update_time = "locations." + std::to_string(player.GetLocation()) + "." + std::to_string(interaction_id) + ".last_updated";
    db::UpdateOneOperation update_post{"players",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(player.GetId())}),
            kvp("player_id",b_int32{player.GetPlayerId()})
        ),
        make_document(kvp("$set",make_document(
            kvp(location_update_time, b_date{modifiedTime})
        )))
    };

    update_post.ExecuteOperation();
    
    return update_post.GetState() == db::OperationState::SUCCESS;
}