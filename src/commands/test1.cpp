#include <test1.h>

test1::test1() 
{

}

void test1::CreateCommand(dpp::commandhandler *handler) {
    this->handler = handler;
    handler->add_command
    (
        "Welcome",
        {
            {"name", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
        },
        [this](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src) {
                std::string got_param;
                if (!parameters.empty()) {
                    got_param = std::get<std::string>(parameters[0].second);
                }
                this->handler->reply(dpp::message("Hello there " + got_param), src);
        },
        "A test ping command"
    );
}