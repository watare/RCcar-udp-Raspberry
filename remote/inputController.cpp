#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>

#include "inputController.h"

InputController::InputController()
{
    ///--- Set the controller to NULL
    m_pGameController=NULL;
    m_iWichController=-1;
 
    ///--- Set the buttons and axis to 0
    memset(m_uButtonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
    memset(m_uButtonStatesPrev, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
    memset(m_fAxisValues, 0, sizeof(float)*SDL_CONTROLLER_AXIS_MAX);
}

bool InputController::Initialize(void)
{
    ///--- Initialize InputController
    SDL_Init(SDL_INIT_GAMECONTROLLER);
 
    ///--- Load the gamecontrollerdb.txt and check if there was any problem
    int iNumOfControllers=SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
    if(iNumOfControllers==-1) {
        SDL_LogWarn(SDL_LOG_CATEGORY_INPUT, "Error loading database [%s]", SDL_GetError());
        return false;
    }
 
    ///--- Ignore the controller events
    SDL_GameControllerEventState(SDL_IGNORE);
    return true;
}

void InputController::Update(void)
{
    ///--- If there is no controllers attached exit
    if(m_pGameController==NULL) {
        return;
    }
 
    ///--- Copy the buttons state to the previous frame info
    memcpy(&m_uButtonStatesPrev, &m_uButtonStates, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
 
    ///--- Update the controller SDL info
    SDL_GameControllerUpdate();
     
    ///--- Obtain the current button values
    for(int b=0; b<SDL_CONTROLLER_BUTTON_MAX; ++b) {
        m_uButtonStates[b]=SDL_GameControllerGetButton(m_pGameController, (SDL_GameControllerButton)b);
    }
    ///--- Obtain the current axis value
    for(int a=0; a<SDL_CONTROLLER_AXIS_MAX; ++a) {
        m_fAxisValues[a]=SDL_GameControllerGetAxis(m_pGameController, (SDL_GameControllerAxis)a);
    }
}

void InputController::ReceiveEvent(const SDL_Event& oEvent)
{
    switch(oEvent.type) {
        ///--- Controller added event
        case SDL_CONTROLLERDEVICEADDED:
        {
            ///--- If there is no controller attached
            if(m_pGameController!=NULL) {
                ///--- Open the controller
                m_iWichController=oEvent.cdevice.which;
                m_pGameController=SDL_GameControllerOpen(m_iWichController);
                ///--- Set the memory to 0 to avoid problems with previous added controllers
                memset(m_uButtonStates, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
                memset(m_uButtonStatesPrev, 0, sizeof(Uint8)*SDL_CONTROLLER_BUTTON_MAX);
                memset(m_fAxisValues, 0, sizeof(float)*SDL_CONTROLLER_AXIS_MAX);
            }
            break;
        }
        ///--- Controller removed event
        case SDL_CONTROLLERDEVICEREMOVED:
        {
            ///--- Check if is the same controller
            if(m_iWichController=oEvent.cdevice.which) {
                m_iWichController=-1;
                m_pGameController=NULL;
            }
            break;
        }
    }
}

bool InputController::IsControllerButtonTriggered(const SDL_GameControllerButton iButton) const
{
    return (m_uButtonStates[iButton]==1&&m_uButtonStatesPrev[iButton]==0);
}

bool InputController::IsControllerButtonPressed(const SDL_GameControllerButton iButton) const
{
    return (m_uButtonStates[iButton]==1);
}

bool InputController::IsControllerButtonReleased(const SDL_GameControllerButton iButton) const
{
    return (m_uButtonStates[iButton]==0&&m_uButtonStatesPrev[iButton]==1);
}


float InputController::GetAxisValue(const SDL_GameControllerAxis iAxis) const
{
    return m_fAxisValues[iAxis];
}