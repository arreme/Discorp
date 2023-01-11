#pragma once
#include <db_instance.hpp>
#include <json.hpp>
#include <user.hpp>
#include <discorp_config.hpp>

using json = nlohmann::json;

namespace db_user
{
    bool DeleteUser(uint64_t user_id);

    bool SaveUser(User user);

    std::unique_ptr<User> FindUserById(uint64_t user_id);

    std::unique_ptr<User[]> FindUsersById(uint64_t user_id);
}
