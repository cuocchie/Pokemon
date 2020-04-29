#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640, ScreenHeight = 480;
const int VelX = 5, VelY = 5;
const int CharacterWidth = 30, CharacterHeight = 30;
const int frameTurn[4] = {0, 1, 0 ,2};
const bool encouterRateBoard[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth, Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
int Position_of_Pointer[2][2] = {{375, 382}, {400, 382}};
int X_Position_of_Poiter = Position_of_Pointer[0][0], Y_Position_of_Pointer = Position_of_Pointer[0][1];
const int Direction = 2, TOTAL_POKEMON = 5;
enum KeyPressesSurFaces
{
    KEY_PRESS_SURFACE_DOWN,//stand: 0
	KEY_PRESS_SURFACE_LEFT,//1
	KEY_PRESS_SURFACE_RIGHT,//2
	KEY_PRESS_SURFACE_UP,//3
	KEY_PRESS_SURFACE_TOTAL//4
};
enum FrameCount
{
    FISRT_FRAME,//0
    SECOND_FRAME,//1
    THIRD_FRAME,//2
    TOTAL_FRAME//3
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

SDL_Rect SpriteClips[KEY_PRESS_SURFACE_TOTAL][TOTAL_FRAME];
SDL_Rect pokemonFighting[TOTAL_POKEMON][Direction];
SDL_Rect playGround;
SDL_Rect currentRect;
SDL_Rect combatBox;
SDL_Rect Pointer;

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
    void moveTexture(SDL_Event& e, int& countFrame, bool& run)
    {
        run = false;
        if(e.key.keysym.sym == SDLK_RIGHT && X_Position_of_Character < 610)
        {
            X_Position_of_Character += VelX;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_RIGHT][frameTurn[countFrame%4]];
            countFrame++;
        }
        else if(e.key.keysym.sym == SDLK_LEFT && X_Position_of_Character > 10)
        {
            X_Position_of_Character -= VelX;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_LEFT][frameTurn[countFrame%4]];
            countFrame++;
        }
        else if(e.key.keysym.sym == SDLK_UP && Y_Position_of_Character > 10)
        {
            Y_Position_of_Character-= VelY;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_UP][frameTurn[countFrame%4]];
            countFrame++;
        }
        else if(e.key.keysym.sym == SDLK_DOWN && Y_Position_of_Character < 430)
        {
            Y_Position_of_Character+= VelY;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][frameTurn[countFrame%4]];
            countFrame++;
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
LTexture gCombatBox;
LTexture gPointer;
LTexture gPokemonFighting;
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

    if(!gSpriteSheetTexture.loadFromFile("data/movement.png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
        success = false;
    }
    else
    {
        //default or down
        for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i ++)
        {

            for(int j = 0; j < TOTAL_FRAME; j++)
            {
                SpriteClips[i][j].x = j*CharacterWidth;
                SpriteClips[i][j].y = i*CharacterHeight;
                SpriteClips[i][j].w = CharacterWidth;
                SpriteClips[i][j].h = CharacterHeight;
            }
        }
    }

    if(!gPokemonFighting.loadFromFile("data/Pokemon.png"))
    {
        cout << "Failed to load Pokemon.png\n";
        success = false;
    }
    else
    {
       for(int i = 0; i < TOTAL_POKEMON; i++)
       {
           for(int j = 0; j < Direction; j++)
           {
               pokemonFighting[i][j].x = j*60;
               pokemonFighting[i][j].y = i*60;
               pokemonFighting[i][j].w = (j + 1)*60;
               if(j == 0) pokemonFighting[i][j].w = 60;
               if(j == 1) pokemonFighting[i][j].w = 150;
               pokemonFighting[i][j].h = 60;
           }
       }
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

    if(!gCombatBox.loadFromFile("data/BattleScreen.png"))
    {
        cout << "Failed to load title\n";
        success = false;
    }
    else
    {
        combatBox.x = 0;
        combatBox.y = 0;
        combatBox.w = ScreenWidth;
        combatBox.h = ScreenHeight;
    }
    if(!gPointer.loadFromFile("data/Pointer.png"))
    {
        cout << "Failed to load Pointer.png\n";
        success = false;
    }
    else
    {
        Pointer.x = 0;
        Pointer.y = 0;
        Pointer.w = 40;
        Pointer.h = 40;
    }
    return success;
}

int generateRandomNumber()
{
    srand(time(0));
    return rand();
}

void close()
{
    gSpriteSheetTexture.free();
    gPlayGround.free();
    gCombatBox.free();
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
            bool run = false;
            SDL_Event  e;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][0];
            SDL_Rect currentCharacterPokemon;
            SDL_Rect currentWildPokemon = {420, 90, 120, 120};
            int countFrame = 0;

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
                        gSpriteSheetTexture.moveTexture(e, countFrame, run);
                    }
                }
                SDL_RenderClear(gRenderer);


                //SDL_RenderPresent(gRenderer);
                gPlayGround.render(0, 0, &playGround);
                gSpriteSheetTexture.render(X_Position_of_Character, Y_Position_of_Character, &currentRect);

                while(Y_Position_of_Character >= 280 && encouterRateBoard[generateRandomNumber()%10] && !run)
                {

                    while(!quit)
                    {
                        gCombatBox.render(0, 0, &combatBox);
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
                                    case SDLK_SPACE:
                                    {
                                        if(X_Position_of_Poiter == Position_of_Pointer[0][0]) run = false;
                                        else run = true;
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

                        if(run == false)
                        {
                            currentWildPokemon = pokemonFighting[0][0];
                            currentCharacterPokemon = pokemonFighting[1][1];
                            gPokemonFighting.render(420, 90, &currentWildPokemon);
                            gPokemonFighting.render(60, 240, &currentCharacterPokemon);
                        }
                        gPointer.render(X_Position_of_Poiter, Y_Position_of_Pointer, &Pointer);
                        if(run || quit) break;
                        SDL_RenderPresent(gRenderer);
                    }
                    if(run || quit) break;
                }

                SDL_RenderPresent(gRenderer);
            }
        }

    }

    close();

    return 0;
}
