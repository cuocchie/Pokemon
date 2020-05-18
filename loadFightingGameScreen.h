#ifndef LOADFIGHTINGGAMESCREEN_H_INCLUDED
#define LOADFIGHTINGGAMESCREEN_H_INCLUDED

#include "globalVar.h"
#include "PokemonInfo.h"
#include "CheckGameEvent.h"
#include "loadMedia.h"

void MovePointer(int& OutLoop, int& moveTurn);
void ShowFightingBox(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon);
void caculateDame(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, const string& whoGoesFirst, int& FightingTextIndex);
void checkFightingEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, PokemonInGameInfo& TempPokemon, int& moveTurn, int& FightingTextIndex, int& PlayerPokemonBall, int& PlayerScore, int& OutLoop, float& catchSuccessfullyRate);
void exercuteFightEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, int& FightingTextIndex, int& OutLoop, int& PlayerScore);
void FightingEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& TempPokemon, LTimer& capTimer,int& PlayerPokemonBall, int& PlayerScore, int& countFrame, int& rate);
#endif // LOADFIGHTINGGAMESCREEN_H_INCLUDED
