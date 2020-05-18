#include "loadChooseTrainerScreen.h"

void ShowGameChoosingCharacter()
{
    gTrainerScreen.render(0, 0, &TrainerScreen);
    gTrainer.render(240, 140, &Trainer[TrainerOrder]);
    SDL_RenderPresent(gRenderer);
    if(GameChecking.QuitChooseTrainer) SDL_Delay(500);
}

void ChooseCharacter()
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
                case SDLK_RIGHT:
                    {
                        if(TrainerOrder == 5)
                        {
                            TrainerOrder = 0;
                            break;
                        }
                        else TrainerOrder ++;
                        Mix_PlayChannel(-1, gMenuEffect, 0);
                        break;
                    }
                case SDLK_LEFT:
                    {
                        if(TrainerOrder == 0)
                        {
                            TrainerOrder = 5;
                            break;
                        }
                        else TrainerOrder --;
                        Mix_PlayChannel(-1, gMenuEffect, 0);
                        break;
                    }
                case SDLK_SPACE:
                    {
                        GameChecking.QuitChooseTrainer = true;
                        gTrainer.render(241, 141, &Trainer[TrainerOrder]);
                        Mix_PlayChannel(-1, gMenuChooseEffect, 0);
                    }
                }
            }
        }
}

void loadGameChoosingPlayer(LTimer& capTimer, int& countFrame)
{

    while(!GameChecking.quit && !GameChecking.QuitChooseTrainer)
    {
        capTimer.start();
        SDL_RenderClear(gRenderer);

        //animation
        GameChecking.animation(10, 0, TrainerScreen);

        ChooseCharacter();
        ShowGameChoosingCharacter();

        countFrame++;
        int frameTicks = capTimer.getTicks();
        if(frameTicks < ScreenTickPerFrame)
        {
            SDL_Delay(ScreenTickPerFrame - frameTicks);
        }
    }

    Mix_HaltMusic();
    Mix_PlayMusic(gLectureMusic, -1);
}
