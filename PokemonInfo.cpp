#include "PokemonInfo.h"

PokemonInGameInfo::PokemonInGameInfo(int _Index, int _level)
{
    Index = _Index;
    currentPokemon = everyPokemon[Index];
    level = _level;
    ATK_IV = rand()%5 + 1;
    DEF_IV = rand()%5 + 1;
    HP_IV = rand()%5 + 1;
    ATK = ((float)(currentPokemon.baseATK + ATK_IV)*2*level/100) + level;
    DEF = ((float)(currentPokemon.baseDEF + DEF_IV)*2*level/100) + level;
    HP = ((float)(currentPokemon.baseHP + HP_IV)*2*level/100) + level + 10;
}
void PokemonInGameInfo::regenerateStats()
{
    ATK = ((float)(currentPokemon.baseATK + ATK_IV)*2*level/100) + level;
    DEF = ((float)(currentPokemon.baseDEF + DEF_IV)*2*level/100) + level;
}

void PokemonInGameInfo::restoreHP()
{
    HP = ((float)(currentPokemon.baseHP + HP_IV)*2*level/100) + level + 10;
}
void PokemonInGameInfo::printStats()
{
    cout <<"name: " <<  currentPokemon.name << " Lv: " << level << endl;
    cout << "IV_ATK, IV_DEF, IV_HP: " << ATK_IV << ' ' << DEF_IV << ' ' << HP_IV << endl;
    cout << "Base ATK,  DEF,    HP: " << currentPokemon.baseATK << ' ' << currentPokemon.baseDEF << ' ' << currentPokemon.baseHP << endl;
    cout << "     ATK,  DEF,    HP: " << ATK << ' ' << DEF << ' ' << HP << endl;
}
