#include<SDL.h>
#include<SDL_image.h>
#include <SDL_ttf.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640, ScreenHeight = 480;
const int ScreenFPS = 60, ScreenTickPerFrame = 10000/ScreenFPS;
const int VelX = 5, VelY = 5;
const int CharacterWidth = 30, CharacterHeight = 30;
const int  frameTurn[4] = {0, 1, 0 ,2};
const bool encouterRateBoard[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth, Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
int Position_of_Pointer[2][2] = {{375, 382}, {400, 382}};
int X_Position_of_Poiter = Position_of_Pointer[0][0], Y_Position_of_Pointer = Position_of_Pointer[0][1];
const int Direction = 2, TOTAL_POKEMON = 5, FrontOfPokemon = 0, BackOfPokemon = 1;
int HealthBarWidth = 144;
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
//Rendered texture
TTF_Font* gFont = NULL;

SDL_Rect MenuScreen, MenuOptions[2][2];
SDL_Rect TrainerScreen, Trainer[6];
SDL_Rect SpriteClips[KEY_PRESS_SURFACE_TOTAL][TOTAL_FRAME];
SDL_Rect pokemonFighting[TOTAL_POKEMON][Direction];
SDL_Rect playGround;
SDL_Rect currentRect;
SDL_Rect combatBox;
SDL_Rect Pointer;
SDL_Rect PlayerHealthBar, WildHealthBar;


class LTimer
{
    public:
		//Initializes variables
		LTimer()
		{
		    mStartTicks = 0;
		    mPausedTicks = 0;

		    mPaused = false;
		    mStarted = false;
		}

		//The various clock actions
		void start()
		{
		    mStarted = true;
		    mPaused = false;
		    mStartTicks = SDL_GetTicks(); //Get the current time
		    mPausedTicks = 0;
		}
		void stop()
		{
		    mStarted = false;
		    mPaused = false;

		    mStartTicks = 0;
		    mPausedTicks = 0;

		}
		void pause()
		{
		    //If the timer is running and isn't already paused
            if( mStarted && !mPaused )
            {
                //Pause the timer
                mPaused = true;

                //Calculate the paused ticks
                mPausedTicks = SDL_GetTicks() - mStartTicks;
                mStartTicks = 0;
            }
		}
		void unpause()
		{
		    //If the timer is running and paused
            if( mStarted && mPaused )
            {
                //Unpause the timer
                mPaused = false;

                //Reset the starting ticks
                mStartTicks = SDL_GetTicks() - mPausedTicks;

                //Reset the paused ticks
                mPausedTicks = 0;
            }
		}

		//Gets the timer's time
		Uint32 getTicks()
		{
		    Uint32 time = 0;
		    if(mStarted)
            {
                if(mPaused)
                {
                    //return the number of ticks when the timer was paused
                    time = mPausedTicks;
                }
                else
                {
                    //return the current time minus the start time
                    time = SDL_GetTicks() - mStartTicks;
                }
            }
            return time;
		}


		//Checks the status of the timer
		bool isStarted()
		{
		    return mStarted;
		}

		bool isPaused()
		{
		   return mPaused && mStarted;
		}

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

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

    bool loadFromRenderedText(string textureText, SDL_Color textColor)
    {
        free();
        //Render text surface

        SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
        if(textSurface == NULL)
        {
            cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
        }
        else
        {
            //Create texture from surface pixels
            mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
            if(mTexture == NULL)
            {
                cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
            }
            else
            {
                //Get image dimensions
                mWidth = textSurface -> w;
                mHeight = textSurface -> h;
            }
            SDL_FreeSurface(textSurface);
        }

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
    void render( int x, int y, SDL_Rect* clip = NULL)
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
    void moveTexture(SDL_Event& e, int& countFrame)
    {
        //run = false;
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
//rendered texture
///text:
LTexture gPLayerLevel, gWildLevel;
///image:
LTexture gSpriteSheetTexture;
LTexture gPlayGround;
LTexture gCombatBox;
LTexture gPointer;
LTexture gPokemonFighting;
LTexture gPlayerHealthBar, gWildHealthBar;
LTexture gMenuScreen, gMenuOptions;
LTexture gTrainerScreen, gTrainer;
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

                //Initialize SDL_ttf
                if(TTF_Init() == -1)
                {
                    cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
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
               pokemonFighting[i][j].x = j*90;
               pokemonFighting[i][j].y = i*120;
               if(j == 0) pokemonFighting[i][j].w = 90;
               if(j == 1) pokemonFighting[i][j].w = 130;
               pokemonFighting[i][j].h = 120;
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

    if(!gWildHealthBar.loadFromFile("data/HealthBar.png"))
    {
        cout << "Failed to load HeathBar.png\n";
        success = false;
    }
    else if(!gPlayerHealthBar.loadFromFile("data/HealthBar.png"))
    {
        cout << "Failed to load HeathBar.png\n";
        success = false;
    }
    else
    {

        PlayerHealthBar = {0, 0, HealthBarWidth, 10};
        WildHealthBar = {0, 0, HealthBarWidth, 10};
    }

    if(!gMenuScreen.loadFromFile("data/Menu.png"))
    {
        cout << "Failed to load Menu.png" << endl;
        success = false;
    }
    else
    {
        MenuScreen = {0, 0, ScreenWidth, ScreenHeight};
    }

    if(!gMenuOptions.loadFromFile("data/MenuOptions.png"))
    {
        cout << "Failed to load MenuOptions.png" << endl;
        success = false;
    }
    else
    {
        for(int i = 0; i < 2; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                MenuOptions[i][j] = {j*130, i*32, 130, 30};
            }
        }
    }
    if(!gTrainerScreen.loadFromFile("data/ChooseTrainer.png"))
    {
        cout << "Failed to load ChooseTrainer.png" << endl;
    }
    else
    {
        TrainerScreen = {0, 0, ScreenWidth, ScreenHeight};
    }
    if(!gTrainer.loadFromFile("data/Trainer.png"))
    {
        cout << "Failed to load Trainer.png" << endl;
    }
    else
    {
        for(int i = 0; i < 6; i++)
        {
            Trainer[i] = {i*200, 0, 200, 300};
        }
    }
    return success;
}

void loadText(string s1, string s2)
{
    gFont = TTF_OpenFont("ARCADECLASSIC.ttf", 38);
    if(gFont == NULL)
    {
        cout << "Failed to load font! SDL_tff Error: " << TTF_GetError() << endl;
    }
    else
    {
        SDL_Color textColor = {0, 0, 0};
        if(!gPLayerLevel.loadFromRenderedText(s1, textColor))
        {
            cout << "Failed to render text texture" << endl;
        }
        else if(!gWildLevel.loadFromRenderedText(s2, textColor))
        {
            cout << "Failed to render text texture" << endl;
        }
    }
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
            bool run = false, fight = false;
            bool PlayerChoosePlay = true, PlayerChooseQuit = false, QuitGameMenu = false, QuitChooseTrainer = false;
            int TrainerOrder = 0;
            SDL_Event  e;
            currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][0];
            SDL_Rect currentCharacterPokemon;
            SDL_Rect currentWildPokemon = {420, 90, 120, 120};
            int PlayerPokemonLevel = 5, WildPokemonLevel;
            int countFrame = 0, Move = 0;

            LTimer fpsTimer;
            LTimer capTimer;
            fpsTimer.start();
            while(!quit)
            {
                //Game Loop
                capTimer.start();
                while(!quit && !QuitGameMenu)
                {
                    ///Loop for loading menu start game
                    capTimer.start();
                    SDL_RenderClear(gRenderer);
                    gMenuScreen.render(0, 0, &MenuScreen);

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
                                    PlayerChoosePlay = !PlayerChoosePlay;
                                    PlayerChooseQuit = !PlayerChooseQuit;
                                    break;
                                }
                                case SDLK_LEFT:
                                {
                                    PlayerChoosePlay = !PlayerChoosePlay;
                                    PlayerChooseQuit = !PlayerChooseQuit;
                                    break;
                                }
                                case SDLK_SPACE:
                                {
                                    if(PlayerChooseQuit) quit = true;
                                    else QuitGameMenu = true;
                                    break;
                                }
                                default:
                                {
                                    quit = false;
                                }
                            }
                        }
                    }

                    gMenuOptions.render(270, 210, &MenuOptions[0][PlayerChoosePlay]);
                    gMenuOptions.render(270, 254, &MenuOptions[1][PlayerChooseQuit]);
                    SDL_RenderPresent(gRenderer);

                    countFrame++;
                    int frameTicks = capTimer.getTicks();
                    if(frameTicks < ScreenTickPerFrame)
                    {
                        SDL_Delay(ScreenTickPerFrame - frameTicks);
                        gTrainerScreen.render(0, 0, &TrainerScreen);
                    }
                }

                while(!quit && !QuitChooseTrainer)
                {
                    capTimer.start();
                    SDL_RenderClear(gRenderer);
                    gTrainerScreen.render(0, 0, &TrainerScreen);

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
                                    if(TrainerOrder == 5)
                                    {
                                        TrainerOrder = 0;
                                        break;
                                    }
                                    else TrainerOrder ++;
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
                                    break;
                                }
                            case SDLK_SPACE:
                                {
                                    QuitChooseTrainer = true;
                                    gTrainer.render(241, 141, &Trainer[TrainerOrder]);
                                    cout << TrainerOrder << endl;
                                }
                            }
                        }
                    }
                    cout << TrainerOrder << endl;
                    gTrainer.render(240, 140, &Trainer[TrainerOrder]);
                    SDL_RenderPresent(gRenderer);
                    if(QuitChooseTrainer) SDL_Delay(500);
                    countFrame++;
                    int frameTicks = capTimer.getTicks();
                    if(frameTicks < ScreenTickPerFrame)
                    {
                        SDL_Delay(ScreenTickPerFrame - frameTicks);
                        gTrainerScreen.render(0, 0, &TrainerScreen);
                    }
                }

                while(SDL_PollEvent(&e) != 0)
                {
                    ///Loop for loading playground and character position
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        run = false;
                        WildHealthBar.w = HealthBarWidth;
                        PlayerHealthBar.w = HealthBarWidth;
                        gSpriteSheetTexture.moveTexture(e, Move);
                    }
                }

                float avgFPS = countFrame/ (fpsTimer.getTicks() /1000.f);
                if(avgFPS > 2000000) avgFPS = 0;

                SDL_RenderClear(gRenderer);

                gPlayGround.render(0, 0, &playGround);
                gSpriteSheetTexture.render(X_Position_of_Character, Y_Position_of_Character, &currentRect);

                while(Y_Position_of_Character >= 280 && encouterRateBoard[generateRandomNumber()%10] && !run)
                {
                    //Loop for combat
                    WildPokemonLevel = generateRandomNumber()%(PlayerPokemonLevel + 1) + 1;
                    while(!quit)
                    {
                        capTimer.start();
                        SDL_RenderClear(gRenderer);
                        gCombatBox.render(0, 0, &combatBox);
                        while(SDL_PollEvent(&e) != 0)
                        {
                            //loop for receive combat status RUN or FIGHT
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
                                        if(X_Position_of_Poiter == Position_of_Pointer[0][0])
                                        {
                                            fight = true;
                                            run = false;
                                        }
                                        else run = true;
                                        break;
                                    }
                                    default:
                                        X_Position_of_Poiter = Position_of_Pointer[0][0];
                                    {
                                        Y_Position_of_Pointer = Position_of_Pointer[0][1];
                                    }
                                }
                            }
                        }
                        gPointer.render(X_Position_of_Poiter, Y_Position_of_Pointer, &Pointer);
                        if(run == false)
                        {

                            currentWildPokemon = pokemonFighting[3][FrontOfPokemon];
                            currentCharacterPokemon = pokemonFighting[4][BackOfPokemon];
                            gPokemonFighting.render(430, 100, &currentWildPokemon);
                            gPokemonFighting.render(90, 240, &currentCharacterPokemon);
                            gWildHealthBar.render(143, 90, &WildHealthBar);
                            gPlayerHealthBar.render(477, 300, &PlayerHealthBar);
                            loadText("Lv " + to_string(PlayerPokemonLevel), "Lv " + to_string(WildPokemonLevel));
                            gPLayerLevel.render(605, 258);
                            gWildLevel.render(270, 48);
                            SDL_RenderPresent(gRenderer);
                            if(fight == true)
                            {
                                WildHealthBar.w -= PlayerPokemonLevel*6;
                                PlayerHealthBar.w -= WildPokemonLevel*6;
                            }
                            fight = false;

                        }
                        if(run || quit) break;
                        SDL_RenderPresent(gRenderer);
                        if(WildHealthBar.w < 0 || PlayerHealthBar.w < 0)
                        {
                            if(WildHealthBar.w < 0 && PlayerHealthBar.w > 0) PlayerPokemonLevel++;
                            run = true;
                        }
                        ///Cap FPS
                        countFrame++;
                        int frameTicks = capTimer.getTicks();
                        if(frameTicks < ScreenTickPerFrame)
                        {
                            SDL_Delay(ScreenTickPerFrame - frameTicks);
                        }
                    }
                    if(run || quit) break;
                }


                SDL_RenderPresent(gRenderer);
                countFrame++;
                int frameTicks = capTimer.getTicks();
                if(frameTicks < ScreenTickPerFrame)
                {
                    SDL_Delay(ScreenTickPerFrame - frameTicks);
                }
            }
        }
    }

    close();

    return 0;
}
