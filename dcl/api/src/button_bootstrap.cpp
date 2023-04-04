#include <api/button_bootstrap.hpp>

ButtonBootstrap::ButtonBootstrap() 
{
    button_list.insert({"next_page",std::make_unique<NextPageButton>()});
    button_list.insert({"previous_page",std::make_unique<PreviousPageButton>()});
}


Button *ButtonBootstrap::Find(std::string button_name) 
{
    return button_list.at(button_name).get();
}