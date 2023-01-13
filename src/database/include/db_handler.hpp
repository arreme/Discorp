#pragma once
#include <db_instance.hpp>
#include <db_access.hpp>
#include <discorp_config.hpp>
#include <db_write.hpp>
#include <user.hpp>
#include <player.hpp>
#include <json.hpp>


using json = nlohmann::json;

namespace db 
{
    bool CreateGameTransaction(User user, Player player);

    std::unique_ptr<User> FindUserById(uint64_t id);
}