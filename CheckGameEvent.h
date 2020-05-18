#ifndef CHECKGAMEEVENT_H_INCLUDED
#define CHECKGAMEEVENT_H_INCLUDED

#include <bits/stdc++.h>
#include <SDL.h>
#include<SDL_mixer.h>
using namespace std;

struct CheckGameEvent
{
    bool quit;
    bool PlayerChooseQuit, PlayerChoosePLay, QuitGameMenu;
    bool QuitChooseTrainer, skip;
    bool encounterNPC, InteractNPC;
    bool run, fight,win, lose, pressCatch, lastLoop, switchable;
    bool zoom;
    //Initialize
    CheckGameEvent();
    void switchButton();
    void ChooseButton();

    void animation(const int Max,const int Min, SDL_Rect& BackGround);
    void resetNPCCollision();
};
extern CheckGameEvent GameChecking;
#endif // CHECKGAMEEVENT_H_INCLUDED
