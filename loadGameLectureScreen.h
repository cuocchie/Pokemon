#ifndef LOADGAMELECTURESCREEN_H_INCLUDED
#define LOADGAMELECTURESCREEN_H_INCLUDED

#include "globalVar.h"
#include "SDLclass.h"
#include "loadMedia.h"
#include "CheckGameEvent.h"

void SwitchChoosingPokemon(int& countText);
void ShowGameLectureScreen(int& countText);
void NextGameLectureSpeech(int& countText);
void loadGameLectureScreen(LTimer& capTimer, int& countFrame);


#endif // LOADGAMELECTURESCREEN_H_INCLUDED
