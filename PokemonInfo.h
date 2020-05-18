#ifndef POKEMONINFO_H_INCLUDED
#define POKEMONINFO_H_INCLUDED

#include <bits/stdc++.h>
#include "globalVar.h"
using namespace std;
struct PokemonBaseInfo
{
    int Index;
    string name;
    int baseATK;
    int baseDEF;
    int baseHP;
};

extern PokemonBaseInfo everyPokemon[TOTAL_POKEMON];// = {{0, PokemonName[0], 49, 49, 45}, {1, PokemonName[1], 52, 43, 39}, {2, PokemonName[2], 48, 65, 44}, {3, PokemonName[3], 45, 40, 40}, {4, PokemonName[4], 56, 35, 30}, {5, PokemonName[5], 35, 55 ,40}, {6, PokemonName[6], 20, 45, 100}, {7, PokemonName[7], 50, 95, 50}, {8, PokemonName[8], 80, 100, 40}, {9, PokemonName[9], 100, 100, 100}};

struct PokemonInGameInfo
{
    int Index;
    PokemonBaseInfo currentPokemon;
    int level;
    int ATK;
    int DEF;
    int HP;
    int ATK_IV;
    int DEF_IV;
    int HP_IV;

    PokemonInGameInfo(int _Index, int _level);
    void regenerateStats();

    void restoreHP();
    void printStats();
};

#endif // POKEMONINFO_H_INCLUDED
