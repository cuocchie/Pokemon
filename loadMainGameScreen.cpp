#include "loadMainGameScreen.h"

bool checkNPCsColision()
{
    bool collised = false;

    switch(e.key.keysym.sym)
    {
    case SDLK_RIGHT:
        {
           if(X_Position_of_Character + CharacterWidth == Position_of_NPC[ProfessorElm][0] && Y_Position_of_Character >= (Position_of_NPC[ProfessorElm][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[ProfessorElm][1] + CharacterHeight))
           {
               meetPro = true;
               collised = true;
           }
           else if(X_Position_of_Character + CharacterWidth == Position_of_NPC[Nurse][0] && Y_Position_of_Character >= (Position_of_NPC[Nurse][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[Nurse][1] + CharacterHeight))
           {
               meetNurse = true;
               collised = true;
           }
           else if(X_Position_of_Character + CharacterWidth == Position_of_NPC[Seller][0] && Y_Position_of_Character >= (Position_of_NPC[Seller][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[Seller][1] + CharacterHeight))
           {
               meetSeller = true;
               collised = true;
           }
           else
           {
               meetPro = false;
               meetNurse = false;
               meetSeller = false;
           }
            return collised;
        }
    case SDLK_LEFT:
        {
            if(X_Position_of_Character - CharacterWidth == Position_of_NPC[ProfessorElm][0] && Y_Position_of_Character >= (Position_of_NPC[ProfessorElm][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[ProfessorElm][1] + CharacterHeight))
            {
                meetPro = true;
                collised = true;
            }
            else if(X_Position_of_Character - CharacterWidth == Position_of_NPC[Nurse][0] && Y_Position_of_Character >= (Position_of_NPC[Nurse][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[Nurse][1] + CharacterHeight))
            {
                meetNurse = true;
                collised = true;
            }
            else if(X_Position_of_Character - CharacterWidth == Position_of_NPC[Seller][0] && Y_Position_of_Character >= (Position_of_NPC[Seller][1] - CharacterHeight) && Y_Position_of_Character <= (Position_of_NPC[Seller][1] + CharacterHeight))
            {
                meetSeller = true;
                collised = true;
            }
            else
            {
                meetPro = false;
                meetNurse = false;
                meetSeller = false;
            }
            return collised;
        }
    case SDLK_UP:
        {
            if(Y_Position_of_Character - CharacterHeight == Position_of_NPC[ProfessorElm][1] && X_Position_of_Character >= (Position_of_NPC[ProfessorElm][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[ProfessorElm][0] + CharacterWidth))
            {
                meetPro = true;
                collised = true;
            }
            else if(Y_Position_of_Character - CharacterHeight == Position_of_NPC[Nurse][1] && X_Position_of_Character >= (Position_of_NPC[Nurse][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[Nurse][0] + CharacterWidth))
            {
                meetNurse = true;
                collised = true;
            }
            else if(Y_Position_of_Character - CharacterHeight == Position_of_NPC[Seller][1] && X_Position_of_Character >= (Position_of_NPC[Seller][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[Seller][0] + CharacterWidth))
            {
                meetSeller = true;
                collised = true;
            }
            else
            {
                meetPro = false;
                meetNurse = false;
                meetSeller = false;
            }
            return collised;
        }
    case SDLK_DOWN:
        {
            if(Y_Position_of_Character + CharacterHeight == Position_of_NPC[ProfessorElm][1] && X_Position_of_Character >= (Position_of_NPC[ProfessorElm][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[ProfessorElm][0] + CharacterWidth))
           {
               meetPro = true;
               collised = true;
           }
            else if(Y_Position_of_Character + CharacterHeight == Position_of_NPC[Nurse][1] && X_Position_of_Character >= (Position_of_NPC[Nurse][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[Nurse][0] + CharacterWidth))
            {
                meetNurse = true;
                collised = true;
            }
            else if(Y_Position_of_Character + CharacterHeight == Position_of_NPC[Seller][1] && X_Position_of_Character >= (Position_of_NPC[Seller][0] - CharacterWidth) && X_Position_of_Character <= (Position_of_NPC[Seller][0] + CharacterWidth))
            {
                meetSeller = true;
                collised = true;
            }
            else
            {
                meetPro = false;
                meetNurse = false;
                meetSeller = false;
            }
        }
    }
    return collised;
}

void MoveCharacter(int& Move, int& NPCDirection, int &rate, PokemonInGameInfo& PlayerPokemon, PokemonInGameInfo& TempPokemon)
{

    if(e.type == SDL_QUIT)
    {
        GameChecking.quit = true;
    }
    else if(e.type == SDL_KEYDOWN)
    {
        GameChecking.run = false;
        WildHealthBar.w = HealthBarWidth;
        rate = rand()%10;
        switch(e.key.keysym.sym)
        {
            case SDLK_RIGHT:
            {
                currentRect = SpriteClips[KEY_PRESS_SURFACE_RIGHT][frameTurn[Move%4]];
                Move++;
                //check encounter NPC:
                if(checkNPCsColision())
                {
                    NPCDirection = KEY_PRESS_SURFACE_LEFT;
                    Move = 0;
                    GameChecking.encounterNPC = true;
                    break;
                }

                if(X_Position_of_Character <= (ScreenWidth/2) - CharacterWidth || camera.x + VelX > ScreenWidth )
                {

                    if(X_Position_of_Character + VelX < ScreenWidth - CharacterWidth)
                        X_Position_of_Character += VelX;
                }
                else
                {
                    camera.x += VelX;
                    Position_of_NPC[0][0] -= VelX;
                    Position_of_NPC[1][0] -= VelX;
                    Position_of_NPC[2][0] -= VelX;
                }
                GameChecking.resetNPCCollision();
                GameChecking.switchable = false;
                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                break;
            }
            case SDLK_LEFT:
            {
                currentRect = SpriteClips[KEY_PRESS_SURFACE_LEFT][frameTurn[Move%4]];
                Move++;
                if(checkNPCsColision())
                {
                    NPCDirection = KEY_PRESS_SURFACE_RIGHT;
                    Move = 0;
                    GameChecking.encounterNPC = true;
                    break;
                }
                if(X_Position_of_Character >= (ScreenWidth/2) - CharacterWidth || camera.x - VelX < 0)
                {
                    if(X_Position_of_Character - VelX > CharacterWidth)
                        X_Position_of_Character -= VelX;
                }
                else
                {
                    camera.x -= VelX;
                    Position_of_NPC[0][0] += VelX;
                    Position_of_NPC[1][0] += VelX;
                    Position_of_NPC[2][0] += VelX;
                }
                GameChecking.resetNPCCollision();
                GameChecking.switchable = false;
                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                break;
            }
            case SDLK_UP:
            {
                currentRect = SpriteClips[KEY_PRESS_SURFACE_UP][frameTurn[Move%4]];
                Move++;
                if(checkNPCsColision())
                {
                    NPCDirection = KEY_PRESS_SURFACE_DOWN;
                    GameChecking.encounterNPC = true;
                    Move = 0;
                    break;
                }
                if(Y_Position_of_Character >= (ScreenHeight/2) - CharacterHeight || camera.y - VelY < 0)
                {
                    if(Y_Position_of_Character - VelY > CharacterHeight)
                        Y_Position_of_Character -= VelY;
                }
                else
                {
                    camera.y -= VelY;
                    Position_of_NPC[0][1] += VelY;
                    Position_of_NPC[1][1] += VelY;
                    Position_of_NPC[2][1] += VelY;
                }
                GameChecking.resetNPCCollision();
                GameChecking.switchable = false;
                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                break;
            }
            case SDLK_DOWN:
            {
                currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][frameTurn[Move%4]];
                Move++;
                if(checkNPCsColision())
                {
                    NPCDirection = KEY_PRESS_SURFACE_UP;
                    GameChecking.encounterNPC = true;
                    Move = 0;
                    break;
                }
                if(Y_Position_of_Character <= (ScreenHeight/2) - CharacterHeight || camera.y + VelY > ScreenHeight)
                {
                    if(Y_Position_of_Character + VelY < ScreenHeight - CharacterHeight)
                    Y_Position_of_Character += VelY;
                }
                else
                {
                    camera.y += VelY;
                    Position_of_NPC[0][1] -= VelY;
                    Position_of_NPC[1][1] -= VelY;
                    Position_of_NPC[2][1] -= VelY;
                }
                GameChecking.resetNPCCollision();
                GameChecking.switchable = false;
                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                else Mix_PlayChannel(-1, gFootStepEffect, 0);
                break;
            }
            case SDLK_SPACE:
            {
                if(GameChecking.encounterNPC)
                {
                     GameChecking.InteractNPC = true;
                     randTips = rand()%5;
                }
                if(GameChecking.switchable)
                {
                    PlayerPokemon = TempPokemon;
                    currentCharacterPokemon = pokemonFighting[PlayerPokemon.Index][BackOfPokemon];
                    GameChecking.switchable = false;
                }
            }
        }
    }
}

void ShowMainGameScreen(int& NPCDirection, int& PlayerScore, int& PlayerPokemonBall)
{
    SDL_RenderClear(gRenderer);
    gPlayGround.render(0, 0, &camera);
    gNPC.render(Position_of_NPC[0][0], Position_of_NPC[0][1], &NPC[NPCDirection][0]);
    gNPC.render(Position_of_NPC[1][0], Position_of_NPC[1][1], &NPC[NPCDirection][1]);
    gNPC.render(Position_of_NPC[2][0], Position_of_NPC[2][1], &NPC[NPCDirection][2]);
    gSpriteSheetTexture.render(X_Position_of_Character, Y_Position_of_Character, &currentRect);
    gScoreBoard.render(0, 0, &ScoreBoard);
    loadText(gPLayerScore, to_string(PlayerScore), 12, WhiteTextColor);
    loadText(gPLayerPokeball, to_string(PlayerPokemonBall), 12, WhiteTextColor);
    gPLayerScore.render(36, 8);
    gPLayerPokeball.render(36, 32);

}

void CheckEncouteringNPC(int& PlayerScore, PokemonInGameInfo& PlayerPokemon, int& PlayerPokemonBalls)
{
    if(GameChecking.InteractNPC && GameChecking.encounterNPC)
    {
        gTextBox.render(60, 380, &TextBox);

        if(meetPro)
        {
            loadText(gNPCsSpeech, ProfSpeech[randTips], 38, GrayTextColor);
        }
        else if(meetNurse)
        {
            PlayerHealthBar.w = HealthBarWidth;
            PlayerPokemon.restoreHP();
            loadText(gNPCsSpeech, NurseSpeech[0], 38, GrayTextColor);
        }
        else if(meetSeller)
        {
            if(PlayerScore < 100) loadText(gNPCsSpeech, SellerSpeech[0], 38, GrayTextColor);
            else
            {
                PlayerScore -= 100;
                PlayerPokemonBalls += 10;
                loadText(gNPCsSpeech, SellerSpeech[0], 38, GrayTextColor);
            }
        }
        gNPCsSpeech.render(90, 390);
    }
}

void loadMainGameScreen(LTimer& capTimer, PokemonInGameInfo& PlayerPokemon ,int& Move, int& NPCDirection, int& countFrame, LTimer fpsTimer)
{
    int PlayerScore = 100;
    int PlayerPokemonBall = 10;
    PokemonInGameInfo TempPokemon(0, 0);
    while(!GameChecking.quit)
    {
        capTimer.start();
        ShowMainGameScreen(NPCDirection, PlayerScore, PlayerPokemonBall);

        int rate;
        float avgFPS = countFrame/ (fpsTimer.getTicks() /1000.f);
        if(avgFPS > 2000000) avgFPS = 0;
        while(SDL_PollEvent(&e) != 0)
        {
            ///Loop for loading playground and character position
            MoveCharacter(Move, NPCDirection, rate, PlayerPokemon, TempPokemon);
        }

        CheckEncouteringNPC(PlayerScore, PlayerPokemon, PlayerPokemonBall);
        FightingEvent(PlayerPokemon,TempPokemon, capTimer, PlayerPokemonBall, PlayerScore, countFrame, rate);

        if(GameChecking.switchable)
        {
            gTextBox.render(60, 380, &TextBox);
            loadText(gAskSwitch, "Press SPACE to switch your Pokemon\ninto " + TempPokemon.currentPokemon.name, 38, GrayTextColor);
            gAskSwitch.render(90, 390);
        }
        if(PlayerScore < 0) PlayerScore = 0;
        SDL_RenderPresent(gRenderer);
        countFrame++;
        int frameTicks = capTimer.getTicks();
        if(frameTicks < ScreenTickPerFrame)
        {
            SDL_Delay(ScreenTickPerFrame - frameTicks);
        }
    }
}
