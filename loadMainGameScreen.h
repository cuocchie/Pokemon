#ifndef LOADMAINGAMESCREEN_H_INCLUDED
#define LOADMAINGAMESCREEN_H_INCLUDED

#include "globalVar.h"
#include "loadMedia.h"
#include "PokemonInfo.h"
#include "CheckGameEvent.h"
#include "loadFightingGameScreen.h"

bool checkNPCsColision();
void MoveCharacter(int& Move, int& NPCDirection, int &rate, PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& TempPokemon);
void ShowMainGameScreen(int& NPCDirection, int& PlayerScore, int& PlayerPokemonBall);
void CheckEncouteringNPC(int& PlayerScore, PokemonInGameInfo& PlayerPokemon, int& PlayerPokemonBalls);
void loadMainGameScreen(LTimer& capTimer, PokemonInGameInfo& PlayerPokemon ,int& Move, int& NPCDirection, int& countFrame, LTimer fpsTimer);

#endif // LOADMAINGAMESCREEN_H_INCLUDED
