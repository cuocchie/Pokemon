#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640;
const int ScreenHeight = 480;
enum KeyPressesSurFaces
{
    KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL

};

bool init();
bool loadMedia();
void close();
//load individual image
SDL_Surface* loadSurface(string path);

SDL_Window* gWindow = NULL;

SDL_Texture* loadTexture(string path);
//Current displayed texture
SDL_Texture* gKeyPressesSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Texture* gTitle = NULL;
SDL_Texture* gCurrentSurface = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;


bool init()
{
    bool success = true;

    //Initial SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            cout << "Warning: Linear texture filtering not enabled!";

        //Create Window
        gWindow = SDL_CreateWindow("PokemonCatcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
        if(gWindow == NULL)
        {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {

            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0xFF, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                // IMG_INIT_PNG = 2
                // IMG_Init(imgFLags) returns 2, then (IMG_Init(imgFlags) & imgFlags) = true
                // !(IMG_Init(imgFlags) & imgFlags) returns not true.
                if(!(IMG_Init(imgFlags) & imgFlags))
                {
                    cout << "SDL_image could not initialize! SDl_image Error: " << IMG_GetError();
                    success = false;
                }
            }

        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

    gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadTexture("data/stand.png");
    gKeyPressesSurfaces[KEY_PRESS_SURFACE_DOWN] = loadTexture("data/stand.png");
    gKeyPressesSurfaces[KEY_PRESS_SURFACE_LEFT] = loadTexture("data/turnLeft.png");
    gKeyPressesSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadTexture("data/turnRight.png");
    gKeyPressesSurfaces[KEY_PRESS_SURFACE_UP] = loadTexture("data/moveUp.png");
    gTitle = loadTexture("data/title.png");
    if(gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_UP] == NULL || gTitle == NULL)
    {
        cout << "Failed to load image!" << endl;
        success = false;
    }
    return success;
}

void close()
{
    for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
    {
        SDL_DestroyTexture(gKeyPressesSurfaces[i]);
        gKeyPressesSurfaces[i] = NULL;
    }

    //SDL_FreeSurface(gStretchedSurface);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(string path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
    {
        cout << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << endl;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL)
            cout << "Unable to create texture " << path << " SDL Error: " << SDL_GetError();

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
int main(int argc, char* argv[])
{
    if(!init())
    {
        cout << "Failed to initialize!" << endl;
    }
    else
    {
        if(!loadMedia())
        {
            cout << "Failed to load media" << endl;
        }
        else
        {
            bool quit = false;
            SDL_Event  e;

            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            while(!quit)
            {
                while(SDL_PollEvent(&e) != 0)
                {

                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        switch(e.key.keysym.sym)
                        {
                        case SDLK_RIGHT:
                            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_RIGHT];
                            break;
                        case SDLK_LEFT:
                            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_LEFT];
                            break;
                        case SDLK_DOWN:
                            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_DOWN];
                            break;
                        case SDLK_UP:
                            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_UP];
                            break;
                        default:
                            gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                            break;
                        }
                    }
                }

                SDL_RenderClear(gRenderer);

                SDL_Rect stretchRect;
                stretchRect.x = 290;
                stretchRect.y = 210;
                stretchRect.w = 60;
                stretchRect.h = 60;
                SDL_RenderSetViewport(gRenderer, &stretchRect);
                SDL_RenderCopy(gRenderer, gCurrentSurface, NULL, NULL);


                SDL_Rect TitleViewport;
                TitleViewport.x = 0;
                TitleViewport.y = 0;
                TitleViewport.w = 125;
                TitleViewport.h = 46;
                SDL_RenderSetViewport(gRenderer, &TitleViewport);
                SDL_RenderCopy(gRenderer, gTitle, NULL, NULL);

                SDL_RenderPresent(gRenderer);
            }
        }

    }

    close();

    return 0;
}
