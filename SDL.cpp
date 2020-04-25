#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640;
const int ScreenHeight = 480;
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int CharacterWidth = 30;
const int CharacterHeight = 30;
int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth;
int Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
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
SDL_Texture* gCurrentSurface = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClips[4];
SDL_Rect playGround;
SDL_Rect currentRect;

class LTexture
{
public:
    //Initializes variables
    LTexture()
    {
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    //Deallocates memory
    ~LTexture()
    {
        free();
    }

    //Loads image at specified path
    bool loadFromFile( std::string path )
    {
        free();

        SDL_Texture* newTexture = NULL;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if(loadedSurface == NULL)
        {
             cout << "Unable to load image " << path << "! SDL Error: " << SDL_GetError() << endl;
        }
        else
        {
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == NULL )
		{
			cout << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
        }

        mTexture = newTexture;
        return mTexture != NULL;
    }

    //Deallocates texture
    void free()
    {
        if(mTexture != NULL)
        {
            SDL_DestroyTexture(mTexture);
            mTexture = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL )
    {
        SDL_Rect renderQuad = {x, y, mWidth, mHeight};

        if(clip != NULL)
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
    }

    //Gets image dimensions
    int getWidth()
    {
        return mWidth;
    }
    int getHeight()
    {
        return mHeight;
    }
    void moveTexture(SDL_Event& e)
    {

        if(e.key.keysym.sym == SDLK_RIGHT && X_Position_of_Character < 610)
        {
            X_Position_of_Character +=25;
        }
        else if(e.key.keysym.sym == SDLK_LEFT && X_Position_of_Character > 10)
        {
            X_Position_of_Character -=25;
        }
        else if(e.key.keysym.sym == SDLK_UP && Y_Position_of_Character > 10)
        {
            Y_Position_of_Character-=25;
        }
        else if(e.key.keysym.sym == SDLK_DOWN && Y_Position_of_Character < 430)
        {
            Y_Position_of_Character+=25;
        }
    }

private:
    SDL_Texture* mTexture;

    //Image dimensions;
    int mWidth;
    int mHeight;

};

LTexture gSpriteSheetTexture;
LTexture gPlayGround;
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
            //gRenderer2 = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL)
            {
                cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
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

    //gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadTexture("data/stand.png");
    //gKeyPressesSurfaces[KEY_PRESS_SURFACE_DOWN] = loadTexture("data/stand.png");
    //gKeyPressesSurfaces[KEY_PRESS_SURFACE_LEFT] = loadTexture("data/turnLeft.png");
    //gKeyPressesSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadTexture("data/turnRight.png");
    //gKeyPressesSurfaces[KEY_PRESS_SURFACE_UP] = loadTexture("data/moveUp.png");
    //gSpriteClips = loadTexture("data/mpvement.png");
    if(!gSpriteSheetTexture.loadFromFile("data/movement.png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
        success = false;
    }
    else
    {
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 30;
        gSpriteClips[0].h = 30;

        gSpriteClips[1].x = 0;
        gSpriteClips[1].y = 30;
        gSpriteClips[1].w = 30;
        gSpriteClips[1].h = 30;

        gSpriteClips[2].x = 0;
        gSpriteClips[2].y = 60;
        gSpriteClips[2].w = 30;
        gSpriteClips[2].h = 30;

        gSpriteClips[3].x = 0;
        gSpriteClips[3].y = 90;
        gSpriteClips[3].w = 30;
        gSpriteClips[3].h = 30;
    }
    if(!gPlayGround.loadFromFile("data/Pokemon_playground.png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
        success = false;
    }
    else
    {
        playGround.x = 0;
        playGround.y = 0;
        playGround.w = ScreenWidth;
        playGround.h = ScreenHeight;
    }
    //gTitle = loadTexture("data/Pokemon_playground.png");
    /*if(gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL || gKeyPressesSurfaces[KEY_PRESS_SURFACE_UP] == NULL || gTitle == NULL)
    {
        cout << "Failed to load image!" << endl;
        success = false;
    }*/
    return success;
}


void close()
{
    gSpriteSheetTexture.free();

    //SDL_FreeSurface(gStretchedSurface);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
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
            //SDL_Rect camera = {0, 0, ScreenWidth, ScreenHeight};
            //gCurrentSurface = gKeyPressesSurfaces[KEY_PRESS_SURFACE_DEFAULT];
            currentRect = gSpriteClips[0];
            while(!quit)
            {
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
                    {
                        switch(e.key.keysym.sym)
                        {
                        case SDLK_RIGHT:
                            gSpriteSheetTexture.moveTexture(e);
                            currentRect = gSpriteClips[2];
                            break;
                        case SDLK_LEFT:
                             gSpriteSheetTexture.moveTexture(e);
                            currentRect = gSpriteClips[1];
                            break;
                        case SDLK_DOWN:
                            gSpriteSheetTexture.moveTexture(e);
                            currentRect = gSpriteClips[0];
                            break;
                        case SDLK_UP:
                             gSpriteSheetTexture.moveTexture(e);
                            currentRect = gSpriteClips[3];
                            break;
                        default:
                            currentRect = gSpriteClips[0];
                            break;
                        }
                    }
                }
                //Title.setViewport(gRenderer, gTitle);
                //character.setViewport(gRenderer, gCurrentSurface);
                SDL_RenderClear(gRenderer);
                gPlayGround.render(0, 0, &playGround);
                gSpriteSheetTexture.render(X_Position_of_Character, Y_Position_of_Character, &currentRect);
                SDL_RenderPresent(gRenderer);
            }
        }

    }

    close();

    return 0;
}
