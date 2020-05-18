#include "loadGameLectureScreen.h"

void SwitchChoosingPokemon(int& countText)
{
    while(SDL_PollEvent(&e) != 0)
    {
        if(e.type == SDL_QUIT)
        {
            GameChecking.quit = true;
        }
        else if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch(e.key.keysym.sym)
            {
                case SDLK_UP:
                {
                    IndexOfTarget = 0;
                    Mix_PlayChannel(-1, gMenuEffect, 0);
                    break;
                }
                case SDLK_DOWN:
                {
                    IndexOfTarget = 1;
                    Mix_PlayChannel(-1, gMenuEffect, 0);
                    break;
                }
                case SDLK_RIGHT:
                {
                    if(IndexOfTarget != 0) IndexOfTarget = 0;
                    else IndexOfTarget = 2;
                    Mix_PlayChannel(-1, gMenuEffect, 0);
                    break;
                }
                case SDLK_LEFT:
                {
                    if(IndexOfTarget != 0) IndexOfTarget = 0;
                    else IndexOfTarget = 2;
                    Mix_PlayChannel(-1, gMenuEffect, 0);
                    break;
                }
                case SDLK_SPACE:
                {
                    countText++;
                    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
                    break;
                }
            }
        }
    }
}

void ShowGameLectureScreen(int& countText)
{
    SDL_RenderClear(gRenderer);
    gLectureBackground.render(0, 0, &LectureBackground);
    gLectureProf.render(0, 0, &LectureProf);
    gLectureSpeech.render(100, 400);
    if(countText == 1) loadText(gLectureSpeech, LectureSpeech[countText] + CharacterName[TrainerOrder] + "!", 38, GrayTextColor);
    else if(countText == 9)
    {
        gPokemonFighting.render(50, 50, &pokemonFighting[0][0]);
        gPokemonFighting.render(100, 150, &pokemonFighting[1][0]);
        gPokemonFighting.render(150, 50, &pokemonFighting[2][0]);

        SwitchChoosingPokemon(countText);
        gTarget.render(Position_of_Target[IndexOfTarget][0], Position_of_Target[IndexOfTarget][1], &Target);
    }
    else if(countText == 10)
    {
        loadText(gLectureSpeech, LectureSpeech[countText] + PokemonName[IndexOfTarget], 38, GrayTextColor);
    }
    else if(countText >= 13)
    {
        GameChecking.skip = true;
    }
    else loadText(gLectureSpeech, LectureSpeech[countText], 38, GrayTextColor);
    SDL_RenderPresent(gRenderer);
}

void NextGameLectureSpeech(int& countText)
{
    while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                GameChecking.quit = true;
            }
            else if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                    countText++;
                    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
                    break;
                case SDLK_e:
                    countText = 9;
                    loadText(gLectureSpeech, LectureSpeech[countText], 38, GrayTextColor);
                    gLectureSpeech.render(100, 400);
                    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
                    break;
                }
            }
        }
}

void loadGameLectureScreen(LTimer& capTimer, int& countFrame)
{
    int countText = 0;
    while(!GameChecking.quit && !GameChecking.skip)
    {
        capTimer.start();

        ShowGameLectureScreen(countText);
        //BackgroundAnimation
        LectureBackground.x += 10;
        if(LectureBackground.x + 10 > (800 - ScreenWidth)) LectureBackground.x = 0;

        NextGameLectureSpeech(countText);

        countFrame++;
        int frameTicks = capTimer.getTicks();
        if(frameTicks < ScreenTickPerFrame)
        {
            SDL_Delay(ScreenTickPerFrame - frameTicks);
        }
    }

    //Change Music
    Mix_HaltMusic();
    Mix_PlayMusic(gIngameMusic, -1);
    //Initialize Character:
    loadCharacter(TrainerOrder);
    currentRect = SpriteClips[0][0];
}
