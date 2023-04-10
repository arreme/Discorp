#include <api/button_bootstrap.hpp>

ButtonBootstrap::ButtonBootstrap(dpp::cluster *bot) 
{
    button_list.insert({"next_page",std::make_unique<NextPageButton>(bot)});
    button_list.insert({"previous_page",std::make_unique<PreviousPageButton>(bot)});
    button_list.insert({"confirm_unlock",std::make_unique<ConfirmUnlockButton>(bot)});
    button_list.insert({"cancel_unlock",std::make_unique<CancelUnlockButton>(bot)});
    button_list.insert({"confirm_improve",std::make_unique<ConfirmImproveButton>(bot)});
    button_list.insert({"cancel_improve",std::make_unique<CancelImproveButton>(bot)});
}


Button *ButtonBootstrap::Find(std::string button_name) 
{
    std::cout << button_name << std::endl;
    return button_list.at(button_name).get();
}