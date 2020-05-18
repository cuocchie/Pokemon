#include "loadMenuScreen.h"

void ChooseMenu()
{
    while(SDL_PollEvent(&e) != 0 && e.key.repeat == 0)
    {
        if(e.type == SDL_QUIT)
        {
            GameChecking.quit = true;
        }
        else if(e.type == SDL_KEYDOWN)
        {
            SDL_Keycode key = e.key.keysym.sym;
            if(key == SDLK_RIGHT || key == SDLK_LEFT || key == SDLK_UP || key == SDLK_DOWN)
            {
                GameChecking.switchButton();
            }
            else if(key == SDLK_SPACE)
            {
                GameChecking.ChooseButton();
            }
        }
    }
}

void ShowMenuScreen()
{
    SDL_RenderClear(gRenderer);
    gMenuScreen.render(0, 0, &MenuScreen);
    gPokemonBanner.render(70, 0, &PokemonBanner);
    gMenuOptions.render(270, 210, &MenuOptions[0][GameChecking.PlayerChoosePLay]);
    gMenuOptions.render(270, 254, &MenuOptions[1][GameChecking.PlayerChooseQuit]);
    SDL_RenderPresent(gRenderer);
}

void loadGameMenuScreen(LTimer& capTimer, int& countFrame)
{
    Mix_PlayMusic(gMenuMusic, -1 );
    while(!GameChecking.quit && !GameChecking.QuitGameMenu)
    {

        ///Loop for loading menu start game
        capTimer.start();

        //animation
        GameChecking.animation(20, 0, MenuScreen);

        ChooseMenu();
        ShowMenuScreen();

        countFrame++;
        int frameTicks = capTimer.getTicks();
        if(frameTicks < ScreenTickPerFrame)
        {
            SDL_Delay(ScreenTickPerFrame - frameTicks);
        }
    }
}
