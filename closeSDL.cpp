#include "closeSDL.h"

void close()
{
    TTF_CloseFont(gFont);
    gFont = NULL;
    //SDL_FreeSurface(gStretchedSurface);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}
