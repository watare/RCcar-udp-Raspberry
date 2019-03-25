#ifndef __INPUTCONTROLLER_H__
#define __INPUTCONTROLLER_H__
#pragma once
///////////////////////////////
 
class InputController
{
public:
    ///--- Constructor
    InputController();
 
    ///--- Object control functions
    bool Initialize(void);
    void Update(void);
    void Release(void);
 
    ///--- To receive the connect and disconnect event
    void ReceiveEvent(const SDL_Event& oEvent);
 
    ///--- Functions to get the state of the buttons
    bool IsControllerButtonTriggered(const SDL_GameControllerButton iButton) const;
    bool IsControllerButtonPressed(const SDL_GameControllerButton iButton) const;
    bool IsControllerButtonReleased(const SDL_GameControllerButton iButton) const;
 
    ///--- Function to get the current value of the axis
    float GetAxisValue(const SDL_GameControllerAxis iAxis) const;
private:
    ///--- Controller information
    SDL_GameController* m_pGameController;
    int m_iWichController;
     
    ///--- Information about the state of the controller
    Uint8 m_uButtonStates[SDL_CONTROLLER_BUTTON_MAX];
    Uint8 m_uButtonStatesPrev[SDL_CONTROLLER_BUTTON_MAX];
    float m_fAxisValues[SDL_CONTROLLER_AXIS_MAX];
};
 
///////////////////////////////
#endif   //__INPUTCONTROLLER_H__