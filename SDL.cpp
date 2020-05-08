#include<SDL.h>
#include<SDL_image.h>
#include <SDL_ttf.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640, ScreenHeight = 480;
const int ScreenFPS = 60, ScreenTickPerFrame = 8000/ScreenFPS;
const int VelX = 5, VelY = 5;
const int CharacterWidth = 40, CharacterHeight = 60;
const int  frameTurn[4] = {0, 1, 0 ,2};
const bool encouterRateBoard[] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
const string CharacterName[] = {"Ethan", "Lyra", "Silver", "Whitney", "Blue", "Karen"};
const string PokemonName[] = {"Bulbasaur", "Charmander", "Squirtle"};
const string LectureSpeech[] = {"Press SPACE to continue or press E to\nskip instruction.","Hello ", "My name is Professor Oak\nWelcome to the Pokemon World!!", "Before head into Pokemon world,\nlet me show you something", "As being a trainer, your ambition is\nbeing one of the best Pokemon trainer.", "To do that, you have to defeat or catch\nPokemon as much as you can.", "But, you need a Pokemon first.", "Now choose your Pokemon.", "You choose a ","And remember, if you need help,\nyou can meet me the right of the map." ,"Okay, now if you are ready press SPACE\nto enter the Pokemon world. Good luck!"};
int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth, Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
int Position_of_Pointer[2][2] = {{375, 382}, {400, 382}};
int Position_of_Target[3][2] = {{40, 80}, {90, 175}, {140, 80}};
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
SDL_Rect LectureBackground;
SDL_Rect LectureProf;
SDL_Rect Target;
SDL_Rect playGround;

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

;
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

    bool loadFromRenderedText(string textureText, SDL_Color textColor, int Wraped)
    {
        free();
        //Render text surface

        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, textureText.c_str(), textColor, Wraped);
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


private:
    SDL_Texture* mTexture;

    //Image dimensions;
    int mWidth;
    int mHeight;

};
//rendered texture
///text:
LTexture gPLayerLevel, gWildLevel;
LTexture gLectureSpeech;
///image:
LTexture gSpriteSheetTexture;
LTexture gPlayGround;
LTexture gCombatBox;
LTexture gPointer;
LTexture gPokemonFighting;
LTexture gPlayerHealthBar, gWildHealthBar;
LTexture gMenuScreen, gMenuOptions;
LTexture gTrainerScreen, gTrainer;
LTexture gLectureBackground;
LTexture gLectureProf;
LTexture gTarget;
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

    if(!gPlayGround.loadFromFile("data/playGround2.png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
        success = false;
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
    if(!gLectureBackground.loadFromFile("data/LectureBackground.png"))
    {
        cout << "Failed to load LectureBackground.png" << endl;
    }
    else
    {
        LectureBackground = {0, 0, ScreenWidth, ScreenHeight};
    }
    if(!gLectureProf.loadFromFile("data/LectureProf.png"))
    {
        cout << "Failed to load LectureProf.png" << endl;
    }
    else
    {
        LectureProf = {0, 0, ScreenWidth, ScreenHeight};
    }
    if(!gTarget.loadFromFile("data/target.png"))
    {
        cout << "Failed to load target.png" << endl;
    }
    else
    {
        Target = {0, 0, 100, 100};
    }
    return success;
}

void loadCharacter(const int& TrainerOrder)
{
    if(!gSpriteSheetTexture.loadFromFile("data/" + CharacterName[TrainerOrder] +".png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
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
}

void loadText(LTexture& gText, string s1, int Wrap, int size)
{
    gFont = TTF_OpenFont("PIXELFONT.ttf", size);
    if(gFont == NULL)
    {
        cout << "Failed to load font! SDL_tff Error: " << TTF_GetError() << endl;
    }
    else
    {
        SDL_Color textColor = {65, 64, 63};
        Uint32 WarpText = 10;
        if(!gText.loadFromRenderedText(s1, textColor, Wrap))
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
            bool skip = false;
            int TrainerOrder = 0;
            SDL_Event  e;
            SDL_Rect currentRect = SpriteClips[0][0];
            SDL_Rect currentCharacterPokemon;
            SDL_Rect camera = {0, 0, ScreenWidth, ScreenHeight};
            SDL_Rect currentWildPokemon = {420, 90, 120, 120};
            int PlayerPokemonLevel = 5, WildPokemonLevel;
            int countFrame = 0, Move = 0, countText = 0;

            LTimer fpsTimer;
            LTimer capTimer;
            fpsTimer.start();
            while(!quit && !QuitChooseTrainer)
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
                                }
                            }
                        }
                    }
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
            }

            int IndexOfTarget = 0;
            cout << sizeof(LectureSpeech);
            while(!quit && !skip)
            {
                capTimer.start();
                SDL_RenderClear(gRenderer);
                gLectureBackground.render(0, 0, &LectureBackground);
                gLectureProf.render(0, 0, &LectureProf);
                gLectureSpeech.render(100, 400);
                if(countText == 1) loadText(gLectureSpeech, LectureSpeech[countText] + CharacterName[TrainerOrder] + "!", 1000, 38);
                else if(countText == 7)
                {
                    gPokemonFighting.render(50, 50, &pokemonFighting[0][0]);
                    gPokemonFighting.render(100, 150, &pokemonFighting[1][0]);
                    gPokemonFighting.render(150, 50, &pokemonFighting[2][0]);

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
                                case SDLK_UP:
                                {
                                    IndexOfTarget = 0;
                                    break;
                                }
                                case SDLK_DOWN:
                                {
                                    IndexOfTarget = 1;
                                    break;
                                }
                                case SDLK_RIGHT:
                                {
                                    if(IndexOfTarget != 0) IndexOfTarget = 0;
                                    else IndexOfTarget = 2;
                                    break;
                                }
                                case SDLK_LEFT:
                                {
                                    if(IndexOfTarget != 0) IndexOfTarget = 0;
                                    else IndexOfTarget = 2;
                                    break;
                                }
                                case SDLK_SPACE:
                                {
                                    countText++;
                                    break;
                                }
                            }

                        }
                    }
                    gTarget.render(Position_of_Target[IndexOfTarget][0], Position_of_Target[IndexOfTarget][1], &Target);
                }
                else if(countText == 8)
                {
                    loadText(gLectureSpeech, LectureSpeech[countText] + PokemonName[IndexOfTarget], 1000, 38);
                }
                else if(countText > 10)
                {
                    skip = true;
                }
                else loadText(gLectureSpeech, LectureSpeech[countText], 1000, 38);
                SDL_RenderPresent(gRenderer);

                //BackgroundAnimation
                LectureBackground.x += 10;
                if(LectureBackground.x + 10 > (800 - ScreenWidth)) LectureBackground.x = 0;

                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
                    {
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_SPACE:
                            countText++;
                            break;
                        case SDLK_e:
                            skip = true;
                            break;
                        }
                    }
                }

                countFrame++;
                int frameTicks = capTimer.getTicks();
                if(frameTicks < ScreenTickPerFrame)
                {
                    SDL_Delay(ScreenTickPerFrame - frameTicks);
                }
            }
            loadCharacter(TrainerOrder);
            currentRect = SpriteClips[0][0];

            while(!quit)
            {
                capTimer.start();
                while(SDL_PollEvent(&e) != 0)
                {
                    ///Loop for loading playground and character position
                    SDL_RenderClear(gRenderer);
                    gPlayGround.render(0, 0, &camera);
                    //loadText("NGu\nNgoc", "NGao ngo a`", 1000, 10);
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        run = false;
                        WildHealthBar.w = HealthBarWidth;
                        PlayerHealthBar.w = HealthBarWidth;

                        switch(e.key.keysym.sym)
                        {
                            case SDLK_RIGHT:
                            {
                                if(X_Position_of_Character <= (ScreenWidth/2) - CharacterWidth || camera.x + VelX > ScreenWidth)
                                {
                                    X_Position_of_Character += VelX;
                                }
                                else
                                {
                                    camera.x += VelX;
                                }

                                currentRect = SpriteClips[KEY_PRESS_SURFACE_RIGHT][frameTurn[Move%4]];
                                Move++;
                                break;
                            }
                            case SDLK_LEFT:
                            {
                                if(X_Position_of_Character >= (ScreenWidth/2) - CharacterWidth || camera.x - VelX < 0)
                                {
                                    X_Position_of_Character -= VelX;
                                }
                                else
                                {
                                    camera.x -= VelX;
                                }
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_LEFT][frameTurn[Move%4]];
                                Move++;
                                break;
                            }
                            case SDLK_UP:
                            {
                                if(Y_Position_of_Character >= (ScreenHeight/2) - CharacterHeight || camera.y - VelY < 0)
                                {
                                    Y_Position_of_Character -= VelY;
                                }
                                else
                                {
                                    camera.y -= VelY;
                                }
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_UP][frameTurn[Move%4]];
                                Move++;
                                break;
                            }
                            case SDLK_DOWN:
                            {
                                if(Y_Position_of_Character <= (ScreenHeight/2) - CharacterHeight || camera.y + VelY > ScreenHeight)
                                {
                                    Y_Position_of_Character += VelY;
                                }
                                else
                                {
                                    camera.y += VelY;
                                }
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][frameTurn[Move%4]];
                                Move++;
                                break;
                            }
                        }
                    }
                }

                float avgFPS = countFrame/ (fpsTimer.getTicks() /1000.f);
                if(avgFPS > 2000000) avgFPS = 0;


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
                                    {
                                        X_Position_of_Poiter = Position_of_Pointer[0][0];
                                        Y_Position_of_Pointer = Position_of_Pointer[0][1];
                                    }
                                }
                            }
                        }
                        gPointer.render(X_Position_of_Poiter, Y_Position_of_Pointer, &Pointer);
                        if(run == false)
                        {

                            currentWildPokemon = pokemonFighting[3][FrontOfPokemon];
                            currentCharacterPokemon = pokemonFighting[IndexOfTarget][BackOfPokemon];
                            gPokemonFighting.render(430, 100, &currentWildPokemon);
                            gPokemonFighting.render(90, 240, &currentCharacterPokemon);
                            gWildHealthBar.render(143, 90, &WildHealthBar);
                            gPlayerHealthBar.render(477, 300, &PlayerHealthBar);
                            //loadText("Lv " + to_string(PlayerPokemonLevel), "Lv \n" + to_string(WildPokemonLevel), 1, 1);
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
