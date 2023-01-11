#pragma once
#include <db_instance.hpp>
#include <db_access.hpp>
#include <discorp_config.hpp>
#include <db_rw.hpp>
#include <user.hpp>
#include <player.hpp>
#include <json.hpp>


using json = nlohmann::json;

namespace db 
{
    int CreateGameTransaction(User user, Player player);
}