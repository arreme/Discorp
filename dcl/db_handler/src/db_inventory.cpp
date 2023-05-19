#include <db_handler/db_inventory.hpp>

bool db_handler::DBInventoryHandler::InitializeInventory(PBUser &user, db::Transaction *t) 
{
    
    db::InsertOneOperation insert_inventory{"inventory",
        make_document(
            kvp("discord_id",b_int64{static_cast<int64_t>(user.discord_id())}),
            kvp("player_id",b_int32{user.current_player_id()}),
            kvp(PBItemType_Name(PBItemType::RESOURCES),b_document{}),
            kvp(PBItemType_Name(PBItemType::ATTACKS),b_document{}),
            kvp(PBItemType_Name(PBItemType::WEAPONS),b_document{}),
            kvp(PBItemType_Name(PBItemType::EQUIPMENT),b_document{}),
            kvp(PBItemType_Name(PBItemType::QUEST_ITEMS),b_document{}),
            kvp(PBItemType_Name(PBItemType::BUILDS),b_document{})
        )
    };

    if (t) 
    {
        t->AddOperation(std::make_unique<db::InsertOneOperation>(insert_inventory));
        return true;
    } else {
        insert_inventory.ExecuteOperation();
        return insert_inventory.GetState() == db::OperationState::SUCCESS;
    }
}

bool db_handler::DBInventoryHandler::UpdateItems(uint64_t discord_id, int32_t player_id) 
{

    bsoncxx::builder::basic::document set_doc{};
    bsoncxx::builder::basic::document inc_doc{};

    for (auto const &item : *m_items)
    {
        int type = item.item_id() / 1000;
        set_doc.append(kvp(PBItemType_Name(type)+"."+std::to_string(item.item_id()),b_int32{item.quantity()}));
    }
    
    db::UpdateOneOperation update_op{"inventory",
        make_document(
            kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        make_document(
            kvp("$set", set_doc.extract())
        )
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::DBInventoryHandler::ModifyItemsQuantity(uint64_t discord_id, int32_t player_id) 
{
    bsoncxx::builder::basic::document set_doc{};
    for (auto const &item : *m_items)
    {
        int type = item.item_id() / 1000;
        set_doc.append(kvp(PBItemType_Name(type)+"."+std::to_string(item.item_id()),b_int32{item.quantity()}));
    }
    db::UpdateOneOperation update_op{"inventory",
        make_document(
            kvp("discord_id", b_int64{static_cast<int64_t>(discord_id)}),
            kvp("player_id", b_int32{player_id})
        ),
        make_document(
            kvp("$inc", set_doc.extract())
        )
    };

    update_op.ExecuteOperation();
    return update_op.GetState() == db::OperationState::SUCCESS;
}

bool db_handler::DBInventoryHandler::GetItems(uint64_t discord_id, int32_t player_id, std::string category) 
{
    bsoncxx::builder::basic::document ids{};
    ids.append(kvp("_id",0));
    for (auto const &item : *m_items)
    {
        int type = item.item_id() / 1000;
        ids.append(kvp(PBItemType_Name(type)+"."+std::to_string(item.item_id()),1));
    }
    
    
    mongocxx::options::find find_options{};
    find_options.projection(
        ids.extract()
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
        
        auto categories = find_op.m_result.value();
        for (auto const &category : categories) 
        {
            for (auto const &elements : category.get_document().view())
            {
                for(auto &item : *m_items) 
                {
                    if (elements.key() == std::to_string(item.item_id())) 
                    {
                        item.set_quantity(elements.get_int32());
                    }
                }
            }
        }
        
        return true;  
    }
    return false;
}

bool db_handler::DBInventoryHandler::GetInventory(uint64_t discord_id, int32_t player_id, std::string category)
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
    m_items->clear();
    if (find_one_op.m_result) 
    {
        auto elements = find_one_op.m_result.value()[category].get_document().value;
        for (auto const &element : elements)
        {
            PBItemData temp;
            temp.set_item_id(static_cast<PBItemEnum>(std::atoi(element.key().data())));
            temp.set_quantity(element.get_int32());
            m_items->push_back(temp);
        }
        return true;
        
    }
    return false;
}