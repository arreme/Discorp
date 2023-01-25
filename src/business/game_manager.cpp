#include <game_manager.hpp>

using namespace g_enums;
using namespace bsoncxx::builder::basic;

/**
* Creates a game
*
* Creates both an initial User and Player instance so they can be added
* to the database
*
* @param discord_id The discord id of the user.
* @param user_name The discord username of the user.
* 
* @return
* DUPLICATED_PK_ERROR if player already inside the database.
* GENERAL_ERROR if transaction couldn't be completed.
* SUCCESS if transaction worked smoothly.
*/
Errors gm::CreateGame(uint64_t discord_id, std::string user_name) 
{
    //Find user
    auto usr_res = db::FindUserById(discord_id);
    if(usr_res) 
    {
        //User not found
        //TODO: User could be found, creating a new player if there are enough slots
        return Errors::DUPLICATED_PK_ERROR;
    }
    
    //User could not be found, initializing brand new game
    User user = User(discord_id,user_name);
    Player player = Player(discord_id,1);
    if(db::CreateGameTransaction(user,player)) 
    {
        return Errors::SUCCESS;
    }

    return Errors::GENERAL_ERROR;
}

Errors gm::DeleteGame(uint64_t discord_id) 
{
    return Errors::SUCCESS;
}

Errors gm::SetActivePlayer(uint64_t discord_id, uint8_t active_player) 
{
    return Errors::SUCCESS;
}

/**
* Go to a specific zone
*
* Checks if the player has unlocked the zone. If it has, updates the player
* current zone inside database.
*
* @param discord_id The discord id of the user.
* @param user_name The discord username of the user.
* @return 
* USER_NOT_FOUND if player not inside the database.
* GENERAL_ERROR if locations could not be found.
* SUCCESS if player has the zone unlocked and the update was correctly pushed.
*/
Errors gm::GoToZone(uint64_t discord_id, g_enums::GameLocations location) 
{
    //Get current player
    auto usr_res = db::FindUserById(discord_id);
    if(!usr_res) return Errors::USER_NOT_FOUND;

    auto locs = db::FindPlayerLocations(discord_id, usr_res->GetCurrentPlayer());
    if(!locs) return Errors::GENERAL_ERROR;

    for (auto curr : *locs)
    {
        if (curr.GetLocId() == location)
        {
            if (db::UpdateCurrentLoc(discord_id,usr_res->GetCurrentPlayer(),location)) 
            {
                return Errors::SUCCESS;
            }
            return Errors::GENERAL_ERROR;
        }
        
    }

    return Errors::ILLEGAL_ACTION;   
}

Errors gm::PhotoCurrentLocation(uint64_t discord_id) 
{
    auto usr_res = db::FindUserById(discord_id);
    if(!usr_res) return Errors::USER_NOT_FOUND;

    auto plyr_res = db::FindPlayerCurrentLocationOnly(discord_id,usr_res->GetCurrentPlayer());

    auto loc_id = plyr_res->GetLocations()->at(0).GetLocId();

    auto loc_doc = utils::LoadLocationInfo(loc_id);

    if(loc_doc.view().length() > 0) 
    {
        auto img_array = loc_doc.view()["images"].get_array().value;
        //calculate loc_doc image
        gdImagePtr img_ptr = utils::ImageLoader("resources/assets/locations/"+img_array[0].get_utf8().value.to_string());
        if(img_ptr != NULL) 
        {
            FILE *fp;
	        fp = fopen("resources/results/res01.png", "wb");
            gdImagePng(img_ptr,fp);
            fclose(fp);
            gdImageDestroy(img_ptr);
            return Errors::SUCCESS;
        }
    }

    return Errors::GENERAL_ERROR;


}

