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

class ConfirmUnlockButton : public Button 
{
    void HandleButton(const dpp::button_click_t & event) override 
    {
        event.command.get_issuing_user();
        dpp::command_completion_event_t callback;
        event.get_original_response(callback);
        
    }
};

class CancelUnlockButton : public Button 
{
    void HandleButton(const dpp::button_click_t & event) override 
    {

    }
};