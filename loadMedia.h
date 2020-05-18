#ifndef LOADMEDIA_H_INCLUDED
#define LOADMEDIA_H_INCLUDED

#include "globalVar.h"
#include "SDLclass.h"

///text:
extern LTexture gPLayerLevel, gWildLevel;
extern LTexture gPLayerScore, gPLayerPokeball;
extern LTexture gLectureSpeech;
extern LTexture gNPCsSpeech;
extern LTexture gFightingSpeech;
extern LTexture gAskSwitch;
///image:
extern LTexture gSpriteSheetTexture;
extern LTexture gNPC;
extern LTexture gPlayGround;
extern LTexture gScoreBoard;
extern LTexture gCombatBox;
extern LTexture gPointer;
extern LTexture gPokemonFighting;
extern LTexture gPlayerHealthBar, gWildHealthBar;
extern LTexture gMenuScreen, gMenuOptions;
extern LTexture gTrainerScreen, gTrainer;
extern LTexture gLectureBackground;
extern LTexture gLectureProf;
extern LTexture gTarget;
extern LTexture gPokemonBanner;
extern LTexture gTextBox;

bool init();
bool loadMedia();
void loadCharacter(const int& TrainerOrder);
void loadText(LTexture& gText, const string s1, const int size, const SDL_Color &textColor);

#endif // LOADMEDIA_H_INCLUDED
