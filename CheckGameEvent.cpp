#include "CheckGameEvent.h"
#include "globalVar.h"
CheckGameEvent::CheckGameEvent()
{
    quit = false;
    zoom = false;
    PlayerChoosePLay = true;
    PlayerChooseQuit = false;
    QuitGameMenu = false;
    QuitChooseTrainer = false;
    skip = false;
    encounterNPC = false;
    InteractNPC = false;
    run = false;
    fight = false;
    win = false;
    lose = false;
    pressCatch = false;
    lastLoop = false;
    switchable = false;
}

void CheckGameEvent::switchButton()
{
    PlayerChoosePLay = !PlayerChoosePLay;
    PlayerChooseQuit = !PlayerChooseQuit;
    Mix_PlayChannel(-1, gMenuEffect, 0);
}

void CheckGameEvent::ChooseButton()
{
    if(PlayerChooseQuit) quit = true;
    else QuitGameMenu = true;
    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
}

void CheckGameEvent::animation(const int Max,const int Min, SDL_Rect& BackGround)
{
    if(BackGround.x == Max && BackGround.y == Max)
    {
        zoom = true;
    }
    else if(BackGround.x == Min && BackGround.y == Min)
    {
        zoom = false;
    }
    if(!zoom)
    {
        BackGround.x += 1;
        BackGround.y += 1;
    }
    else
    {
        BackGround.x -= 1;
        BackGround.y -= 1;
    }
}

void CheckGameEvent::resetNPCCollision()
{
    InteractNPC = false;
    encounterNPC = false;
}
