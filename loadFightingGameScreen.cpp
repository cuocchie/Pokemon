#include "loadFightingGameScreen.h"

void MovePointer(int& OutLoop, int& moveTurn)
{
    while(SDL_PollEvent(&e) != 0 && e.key.repeat == 0 && (OutLoop == 2) && moveTurn == 0)
    {
        //loop for receive combat status RUN or FIGHT
        if(e.type == SDL_QUIT)
        {
            GameChecking.quit = true;
        }

        else if(e.type == SDL_KEYDOWN)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_RIGHT:
                {
                    X_Position_of_Poiter = Position_of_Pointer[1][0];
                    Y_Position_of_Pointer = Position_of_Pointer[1][1];
                    break;
                }
                case SDLK_LEFT:
                {
                    X_Position_of_Poiter = Position_of_Pointer[0][0];
                    Y_Position_of_Pointer = Position_of_Pointer[0][1];
                    break;
                }
                case SDLK_DOWN:
                {
                    X_Position_of_Poiter = Position_of_Pointer[2][0];
                    Y_Position_of_Pointer = Position_of_Pointer[2][1];
                    break;
                }
                case SDLK_UP:
                {
                    X_Position_of_Poiter = Position_of_Pointer[0][0];
                    Y_Position_of_Pointer = Position_of_Pointer[0][1];
                    break;
                }
                case SDLK_SPACE:
                {
                    if(X_Position_of_Poiter == Position_of_Pointer[0][0] )
                    {
                        if(Y_Position_of_Pointer == Position_of_Pointer[0][1])
                        {
                            GameChecking.fight = true;
                            GameChecking.run = false;
                        }
                        else
                        {
                            GameChecking.pressCatch = true;
                            GameChecking.run = false;
                        }
                    }
                    else
                    {
                        GameChecking.run = true;
                    }
                    break;
                }
                default:
                {
                    X_Position_of_Poiter = Position_of_Pointer[0][0];
                    Y_Position_of_Pointer = Position_of_Pointer[0][1];
                }
            }
        }
    }
}

void ShowFightingBox(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon)
{
    SDL_RenderClear(gRenderer);
    gCombatBox.render(0, 0, &combatBox);
    gPointer.render(X_Position_of_Poiter, Y_Position_of_Pointer, &Pointer);
    gPokemonFighting.render(430, 100, &currentWildPokemon);
    gPokemonFighting.render(90, 240, &currentCharacterPokemon);
    gWildHealthBar.render(143, 90, &WildHealthBar);
    gPlayerHealthBar.render(477, 300, &PlayerHealthBar);
    loadText(gWildLevel,"Lv " + to_string(WildPokemon.level), 38, GrayTextColor);
    loadText(gPLayerLevel, "Lv "  + to_string(PlayerPokemon.level), 38, GrayTextColor);
    gPLayerLevel.render(570, 258);
    gWildLevel.render(240, 48);
}

void caculateDame(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, const string& whoGoesFirst, int& FightingTextIndex)
{
    if(whoGoesFirst == "Player")
    {
        int critRate = rand()%3 + 1;
        int dame = PlayerPokemon.ATK - WildPokemon.DEF/critRate;
        if(dame <= 0) dame = 1;
        float HPdecrease = ((float)dame/WildPokemon.HP)*HealthBarWidth;
        WildPokemon.HP -= dame;
        WildHealthBar.w -= HPdecrease;
        FightingTextIndex = PLAYER_FIGHT;
    }
    else if(whoGoesFirst == "Wild")
    {
        int critRate = rand()%3 + 1;
        float dame = WildPokemon.ATK - PlayerPokemon.DEF/critRate;
        if(dame <= 0) dame = 1;
        int HPdecrease = ((float)dame/PlayerPokemon.HP)*HealthBarWidth;
        PlayerPokemon.HP -= dame;
        PlayerHealthBar.w -= HPdecrease;
        FightingTextIndex = WILD_FIGHT;
    }
}

void checkFightingEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, PokemonInGameInfo& TempPokemon, int& moveTurn, int& FightingTextIndex, int& PlayerPokemonBall, int& PlayerScore, int& OutLoop, float& catchSuccessfullyRate)
{
    if(!GameChecking.run)
    {
        if(GameChecking.fight)
        {
            if(PlayerPokemon.level >= WildPokemon.level)
            {
                if(moveTurn == 0)
                {
                    caculateDame(PlayerPokemon, WildPokemon, "Player", FightingTextIndex);
                }
                else if(moveTurn == 1)
                {
                    caculateDame(PlayerPokemon, WildPokemon, "Wild", FightingTextIndex);
                }
            }
            else
            {
                if(moveTurn == 0)
                {
                    caculateDame(PlayerPokemon, WildPokemon, "Wild", FightingTextIndex);
                }
                else if(moveTurn == 1)
                {
                   caculateDame(PlayerPokemon, WildPokemon, "Player", FightingTextIndex);
                }
            }
            moveTurn++;
            if(moveTurn == 3)
            {
                 GameChecking.fight = false;
                 FightingTextIndex = WAIT_FOR_MOVE;
                 moveTurn = 0;
            }
        }
        else if(GameChecking.pressCatch && PlayerPokemonBall > 0)
        {
            int catchRate = rand()%100 + 1;
            if(catchRate < catchSuccessfullyRate)
            {
                FightingTextIndex = PLAYER_CAUGHT;
                GameChecking.switchable = true;
                PlayerPokemonBall--;
                PlayerScore += WildPokemon.level*5 + BonusPoint*(WildPokemon.level > PlayerPokemon.level);
                TempPokemon = WildPokemon;
                TempPokemon.printStats();
                GameChecking.run = true;
            }
            else
            {
                FightingTextIndex = PLAYER_MISSED;
                int critRate = rand()%3 + 1;
                int dame = WildPokemon.ATK - PlayerPokemon.DEF/critRate;
                if(dame <= 0) dame = 1;
                int HPdecrease = ((float)dame/PlayerPokemon.HP)*HealthBarWidth;
                PlayerPokemon.HP -= dame;
                PlayerHealthBar.w -= HPdecrease;
            }
            GameChecking.pressCatch = false;

        }
        else if(GameChecking.pressCatch && PlayerPokemonBall <= 0)
        {
            FightingTextIndex = PLAYER_OUT_OF_BALL;
            GameChecking.pressCatch = false;
        }

        if(WildHealthBar.w <= 0 || PlayerHealthBar.w <= 0)
        {
            if(WildHealthBar.w <= 0 && PlayerHealthBar.w > 0)
            {
                FightingTextIndex = PLAYER_WIN;
                GameChecking.win = true;

            }
            else if(WildHealthBar.w >= 0 && PlayerHealthBar.w <= 0)
            {
                FightingTextIndex = PLAYER_LOSE;
                GameChecking.lose = true;
            }
            -- OutLoop;
        }
    }
    else FightingTextIndex = PLAYER_RAN;
}

void exercuteFightEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& WildPokemon, int& FightingTextIndex, int& OutLoop, int& PlayerScore)
{
    if(FightingTextIndex == FIRST_ENCOUNTER_TEXT || FightingTextIndex == PLAYER_CAUGHT) loadText(gFightingSpeech, PokemonEncounterText[FightingTextIndex] + WildPokemon.currentPokemon.name, 38, GrayTextColor);
    else if(FightingTextIndex == PLAYER_WIN)
    {
        Mix_PlayChannel(-1, gLevelUp, 0);
        loadText(gFightingSpeech,PokemonEncounterText[FightingTextIndex] + to_string(PlayerPokemon.level + 1), 38, GrayTextColor);
    }
    else loadText(gFightingSpeech, PokemonEncounterText[FightingTextIndex], 38, GrayTextColor);

    gFightingSpeech.render(15, 380);

    SDL_RenderPresent(gRenderer);
    if(FightingTextIndex == PLAYER_FIGHT || FightingTextIndex == WILD_FIGHT)
    {
        Mix_PlayChannel(-1, gTackleSound, 0);
        SDL_Delay(1000);
    }
    else if(FightingTextIndex == PLAYER_MISSED)
    {
        SDL_Delay(1000);
        FightingTextIndex = WAIT_FOR_MOVE;
    }
    else if(FightingTextIndex == PLAYER_LOSE)
    {
        camera.x = 315;
        camera.y = 300;
        Position_of_NPC[0][0] = -215;
        Position_of_NPC[0][1] = -200;
        Position_of_NPC[1][0] = 325;
        Position_of_NPC[1][1] = 180;
        Position_of_NPC[2][0] = 735;
        Position_of_NPC[2][1] = -200;
        Mix_PlayChannel(-1, gLoseSoundEffect, 0);
    }
    if(OutLoop == 0) GameChecking.run = true;
    if(GameChecking.run || GameChecking.quit)
    {
        if(GameChecking.win)
        {
            PlayerPokemon.level++;
            PlayerScore += WildPokemon.level*2 + BonusPoint*(WildPokemon.level > WildPokemon.level);
        }
        else if(GameChecking.lose)
        {
            PlayerScore -= (PlayerPokemon.level - WildPokemon.level + 2)*2;
        }
        GameChecking.win = false;
        GameChecking.lose = false;
        GameChecking.fight = false;
        GameChecking.pressCatch = false;
        SDL_Delay(2000);
    }
}

void FightingEvent(PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& TempPokemon, LTimer& capTimer,int& PlayerPokemonBall, int& PlayerScore, int& countFrame, int& rate)
{
    int FightingTextIndex = 0;
    int moveTurn = 0;
    currentCharacterPokemon = pokemonFighting[PlayerPokemon.Index][BackOfPokemon];
    int OutLoop = 2;
    if(camera.y >= 320  && (rate == 5) && !GameChecking.run)
    {
        PlayerPokemon.regenerateStats();

        int randomPokemon = rand()%(TOTAL_POKEMON - 1);
        int wildPokemonLevel = rand()%(PlayerPokemon.level + 1) + 1;
        if(rand()%100 == 69)
        {
            randomPokemon = 9;
            wildPokemonLevel = 50;
        }
        currentWildPokemon = pokemonFighting[randomPokemon][FrontOfPokemon];
        PokemonInGameInfo WildPokemon(randomPokemon, wildPokemonLevel);
        //Loop for combat

        Mix_PauseMusic();
        Mix_VolumeChunk(gBattleMusic, MIX_MAX_VOLUME/2);
        Mix_PlayChannel(-1, gBattleMusic, -1);
        while(!GameChecking.quit && !GameChecking.run)
        {
            float catchSuccessfullyRate = 100*(((float)HealthBarWidth - (float)WildHealthBar.w)/(float)HealthBarWidth);
            capTimer.start();

            MovePointer(OutLoop, moveTurn);
            ShowFightingBox(PlayerPokemon, WildPokemon);
            checkFightingEvent(PlayerPokemon, WildPokemon, TempPokemon, moveTurn, FightingTextIndex, PlayerPokemonBall, PlayerScore, OutLoop, catchSuccessfullyRate);
            exercuteFightEvent(PlayerPokemon, WildPokemon, FightingTextIndex, OutLoop, PlayerScore);

            ///Cap FPS
            countFrame++;
            int frameTicks = capTimer.getTicks();
            if(frameTicks < ScreenTickPerFrame)
            {
                SDL_Delay(ScreenTickPerFrame - frameTicks);
            }
        }
        if(GameChecking.run || GameChecking.quit)
        {
            Mix_HaltChannel(-1);
            Mix_ResumeMusic();
        }

    }
}
