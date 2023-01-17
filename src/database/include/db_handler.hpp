#pragma once
#include <db_instance.hpp>
#include <db_access.hpp>
#include <discorp_config.hpp>
#include <db_write.hpp>
#include <user.hpp>
#include <player.hpp>
#include <db_query.hpp>
#include <location.hpp>
#include <mongocxx/pipeline.hpp>

namespace db 
{
    bool CreateGameTransaction(User user, Player player);

    std::unique_ptr<User> FindUserById(uint64_t id);

    std::tuple<User,Player,Location> FindPlayerUserAndLocation(uint64_t discord_id);
}