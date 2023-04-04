#pragma once
#include <dpp.h>
#include <memory>
#include <core/game_manager.hpp>

class Button 
{
public:
    virtual void HandleButton(const dpp::button_click_t & event) = 0;
};

class NextPageButton : public Button
{
public:
    void HandleButton(const dpp::button_click_t & event) override 
    {
        auto user = event.command.get_issuing_user();
        //gm::NextPage(event);
    }
};

class PreviousPageButton : public Button 
{
    void HandleButton(const dpp::button_click_t & event) override 
    {

    }
};