#include <db_handler/db_handler.hpp>

using bsoncxx::builder::basic::kvp;

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