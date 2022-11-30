#include <login.h>

Login::Login() 
{

}

void Login::CreateCommand(dpp::commandhandler *handler) 
{
    this->handler = handler;
    handler->add_command
    (
        "login",
        {
            {"name", dpp::param_info(dpp::pt_string, true, "Leave blank if want to use current discord name") }
        },
        [this](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src) 
        {
            
        },
        "Creates a game inside Discorp. You won't be able to edit the name so choose wisely!"
    );
}