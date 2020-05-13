#include<SDL.h>
#include<SDL_image.h>
#include <SDL_ttf.h>
#include<SDL_mixer.h>
#include<bits/stdc++.h>
using namespace std;

const int ScreenWidth = 640, ScreenHeight = 480;
const int ScreenFPS = 60, ScreenTickPerFrame = 8000/ScreenFPS;
const int VelX = 5, VelY = 5;
const int CharacterWidth = 40, CharacterHeight = 60;
const int NumberOfNPCs = 3;
const int  frameTurn[4] = {0, 1, 0 ,2}, TOTAL_FRAME = 3;
const string CharacterName[] = {"Ethan", "Lyra", "Silver", "Whitney", "Blue", "Karen"};
const string PokemonName[] = {"Bulbasaur", "Charmander", "Squirtle","Pidgey", "Rattata"};
const string LectureSpeech[] = {"Press SPACE to continue or press E to\nskip instruction.","Hello ", "My name is Professor Oak\nWelcome to the Pokemon World!!", "Before head into Pokemon world,\nlet me show you something", "As being a trainer, your ambition is\nbeing one of the best Pokemon trainer.", "To do that, you have to defeat or catch\nPokemon as much as you can.", "But, you need a Pokemon first.", "Now choose your Pokemon.", "You choose a ","And remember, if you need help,\nyou can meet me the right of the map." ,"Okay, now if you are ready press SPACE\nto enter the Pokemon world. Good luck!"};
const string ProfSpeech[] = {"Hi, I am Professor Elm!", "You are new here."};
const string NurseSpeech[] = {"Your Pokemon is fully healed"};
const string SellerSpeech[] = {"You don't have enough money", "You got 10 pokeballs for 100$"};
const string PokemonEncounterText[] = {"You encountered a wild\n", "You've run away", "Your Pokemon used Tackle", "Your Pokemon wins", "Your Pokemon Loses", "You caught a wild\n", "Missed", "You are out of Ball"};
int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth, Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
int Position_of_Pointer[3][2] = {{345, 375}, {485, 375}, {345, 420}};
int Position_of_Target[3][2] = {{40, 80}, {90, 175}, {140, 80}};
int Position_of_NPC[NumberOfNPCs][2] = {{100, 100}, {640, 480}, {1050, 100}};
bool meetPro = false, meetNurse = false, meetSeller = false;
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
enum FightingText
{
    FIRST_ENCOUNTER_TEXT,//0
    WAIT_FOR_PLAYERMOVE,//1
    PLAYER_CHOOSE_FIGHT,//2
    PLAYER_WIN, //3
    PLAYER_LOSE, //4
    PLAYER_CAUGHT, //5
    PLAYER_MISSED, //6
    PLAYER_OUT_OF_BALL//7
};
enum NPCs
{
    ProfessorElm,
    Nurse,
    Seller,
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

//The music that will be played
Mix_Music *gMenuMusic= NULL;
Mix_Music *gLectureMusic = NULL;
Mix_Music *gIngameMusic = NULL;
Mix_Chunk *gBattleMusic = NULL;
//The sound effects that will be used
Mix_Chunk *gMenuEffect = NULL;
Mix_Chunk *gMenuChooseEffect = NULL;
Mix_Chunk *gFootStepEffect = NULL;
Mix_Chunk *gFootStepBushEffect = NULL;

SDL_Rect MenuScreen, MenuOptions[2][2];
SDL_Rect PokemonBanner;
SDL_Rect TrainerScreen, Trainer[6];
SDL_Rect SpriteClips[KEY_PRESS_SURFACE_TOTAL][TOTAL_FRAME];
SDL_Rect NPC[KEY_PRESS_SURFACE_TOTAL][3];
SDL_Rect pokemonFighting[TOTAL_POKEMON][Direction];
SDL_Rect ScoreBoard;
SDL_Rect LectureBackground;
SDL_Rect LectureProf;
SDL_Rect Target;
SDL_Rect playGround;
SDL_Rect TextBox;
SDL_Rect combatBox;
SDL_Rect Pointer;
SDL_Rect PlayerHealthBar, WildHealthBar;

//color
SDL_Color GrayTextColor = {65, 64, 63};
SDL_Color ShadowTextColor = {215, 214, 213};
SDL_Color WhiteTextColor = {255, 255, 255};


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
LTexture gPLayerScore, gPLayerPokeball;
LTexture gLectureSpeech;
LTexture gNPCsSpeech;
LTexture gFightingSpeech;
///image:
LTexture gSpriteSheetTexture;
LTexture gNPC;
LTexture gPlayGround;
LTexture gScoreBoard;
LTexture gCombatBox;
LTexture gPointer;
LTexture gPokemonFighting;
LTexture gPlayerHealthBar, gWildHealthBar;
LTexture gMenuScreen, gMenuOptions;
LTexture gTrainerScreen, gTrainer;
LTexture gLectureBackground;
LTexture gLectureProf;
LTexture gTarget;
LTexture gPokemonBanner;
LTexture gTextBox;
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
                //Initialize SDL_mixer
                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
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
    if(!gNPC.loadFromFile("data/NPC.png"))
    {
        cout << "Failed to load NPC.png!\n";
        success = false;
    }
    else
    {
        for(int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++)
        {
            for(int j = 0; j < NumberOfNPCs; j++)
            {
                NPC[i][j].x = j*40;
                NPC[i][j].y = i*60;
                NPC[i][j].w = CharacterWidth;
                NPC[i][j].h = CharacterHeight;
            }
        }
    }
    if(!gPlayGround.loadFromFile("data/playGround2.png"))
    {
        cout << "Failed to load sprite sheet texture!\n";
        success = false;
    }
    if(!gScoreBoard.loadFromFile("data/ScoreBoard.png"))
    {
        cout << "Failed to load ScoreBoard!\n";
        success = false;
    }
    else
    {
        ScoreBoard = {0, 0, 120, 50};
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
    if(!gPokemonBanner.loadFromFile("data/PokemonBanner.png"))
    {
        cout << "Failed to load PokemonBanner.png" << endl;
        success = false;
    }
    else
    {
        PokemonBanner = {0, 0, 480, 180};
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
    if(!gTextBox.loadFromFile("data/textBox.png"))
    {
        cout << "Failed to load TextBox.png\n";
    }
    else
    {
        TextBox = {0, 0, 540, 90};
    }
    //Load music
    //Mix_Music *gMenuMusic= NULL;
    //Mix_Music *gLectureMusic = NULL;
    //Mix_Music *gIngameMusic = NULL;
    gMenuMusic = Mix_LoadMUS("Sound/MainMenuMusic.mp3");
    if(gMenuMusic == NULL)
    {
        cout << "Failed to load beat music! SDL_mixer Error: " << Mix_GetError() << endl;
        success = false;
    }
    gLectureMusic = Mix_LoadMUS("Sound/LectureMusic.mp3");
    if(gLectureMusic == NULL)
    {
        cout << "Failed to load Lecture Music! SDL_Mixer Error: " << Mix_GetError() << endl;
    }
    gIngameMusic = Mix_LoadMUS("Sound/InGameMusic.mp3");
    if(gIngameMusic == NULL)
    {
        cout << "Failed to load In Game music! SDL_Mixer Error: " << Mix_GetError() << endl;
    }
    gBattleMusic = Mix_LoadWAV("Sound/BattleMusic.mp3");
    if(gBattleMusic == NULL)
    {
        cout << "Failed to load Battle Music! SDL_mixer error: " << Mix_GetError() << endl;
    }
    //Load sound effects
    gMenuEffect = Mix_LoadWAV("Sound/SFX Menu.wav");
    if(gMenuEffect == NULL)
    {
        cout << "Failed to load Menu SFX! SDL_mixer error: " << Mix_GetError() << endl;
    }
    gMenuChooseEffect = Mix_LoadWAV("Sound/SFX Choose.wav");
    if(gMenuChooseEffect == NULL)
    {
        cout << "Failed to load Menu Choose SFX! SDL_mixer error: " << Mix_GetError() << endl;
    }
    gFootStepEffect = Mix_LoadWAV("Sound/SFX FootStep.mp3");
    if(gFootStepEffect == NULL)
    {
        cout << "Failed to load FootStep SFX! SDL_mixer error: " << Mix_GetError() << endl;
    }
    gFootStepBushEffect = Mix_LoadWAV("Sound/SFX FootStepBush.mp3");
    if(gFootStepBushEffect == NULL)
    {
        cout << "Failed to load FootStepBush SFX! SDL_mixer error: " << Mix_GetError() << endl;
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

void loadText(LTexture& gText, const string s1, const int size, const SDL_Color &textColor)
{
    gFont = TTF_OpenFont("PIXELFONT.ttf", size);
    if(gFont == NULL)
    {
        cout << "Failed to load font! SDL_tff Error: " << TTF_GetError() << endl;
    }
    else
    {
        if(!gText.loadFromRenderedText(s1, textColor, 1000))
        {
            cout << "Failed to render text texture" << endl;
        }
    }
}

bool checkNPCsColision(SDL_Event& e)
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
        return collised;
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
            bool quit = false, zoom = false;
            bool run = false, fight = false, pressCatch = false;
            bool PlayerChoosePlay = true, PlayerChooseQuit = false, QuitGameMenu = false, QuitChooseTrainer = false;
            bool skip = false;
            int TrainerOrder = 0;
            int NPCDirection = KEY_PRESS_SURFACE_DOWN;
            bool encouterNPC = false, interactNPC = false;
            SDL_Event  e;
            SDL_Rect currentRect = SpriteClips[0][0];
            SDL_Rect currentCharacterPokemon;
            SDL_Rect camera = {0, 0, ScreenWidth, ScreenHeight};
            SDL_Rect currentWildPokemon = {420, 90, 120, 120};
            int PlayerScore = 0, PlayerPokemonBall = 10;
            int PlayerPokemonLevel = 5, WildPokemonLevel;
            int countFrame = 0, Move = 0, countText = 0;
            int IndexOfTarget = 0;
            LTimer fpsTimer;
            LTimer capTimer;
            fpsTimer.start();
            while(!quit && !QuitChooseTrainer)
            {
                //Game Loop
                capTimer.start();
                Mix_PlayMusic( gMenuMusic, -1 );
                while(!quit && !QuitGameMenu)
                {
                    ///Loop for loading menu start game
                    capTimer.start();

                    SDL_RenderClear(gRenderer);
                    gMenuScreen.render(0, 0, &MenuScreen);
                    gPokemonBanner.render(70, 0, &PokemonBanner);
                    //animation
                    if(MenuScreen.x == 20 && MenuScreen.y == 20)
                    {
                        zoom = true;
                    }
                    else if(MenuScreen.x == 0 && MenuScreen.y ==0)
                    {
                        zoom = false;
                    }
                    if(!zoom)
                    {
                        MenuScreen.x += 1;
                        MenuScreen.y += 1;
                    }
                    else
                    {
                        MenuScreen.x -= 1;
                        MenuScreen.y -= 1;
                    }

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
                                    Mix_PlayChannel(-1, gMenuEffect, 0);
                                    break;
                                }
                                case SDLK_LEFT:
                                {
                                    PlayerChoosePlay = !PlayerChoosePlay;
                                    PlayerChooseQuit = !PlayerChooseQuit;
                                    Mix_PlayChannel(-1, gMenuEffect, 0);
                                    break;
                                }
                                case SDLK_SPACE:
                                {
                                    if(PlayerChooseQuit) quit = true;
                                    else QuitGameMenu = true;
                                    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
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
                    }
                }


                while(!quit && !QuitChooseTrainer)
                {
                    capTimer.start();
                    SDL_RenderClear(gRenderer);
                    gTrainerScreen.render(0, 0, &TrainerScreen);
                    //animation
                    if(TrainerScreen.h == ScreenHeight + 10)
                    {
                        zoom = false;
                    }
                    else if(TrainerScreen.h == ScreenHeight)
                    {
                        zoom = true;
                    }
                    if(zoom) TrainerScreen.h += 1;
                    else TrainerScreen.h -= 1;

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
                                    QuitChooseTrainer = true;
                                    gTrainer.render(241, 141, &Trainer[TrainerOrder]);
                                    Mix_PlayChannel(-1, gMenuChooseEffect, 0);
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
                    }
                }
            }

            Mix_HaltMusic();
            Mix_PlayMusic(gLectureMusic, -1);
            while(!quit && !skip)
            {
                capTimer.start();
                SDL_RenderClear(gRenderer);
                gLectureBackground.render(0, 0, &LectureBackground);
                gLectureProf.render(0, 0, &LectureProf);
                gLectureSpeech.render(100, 400);
                if(countText == 1) loadText(gLectureSpeech, LectureSpeech[countText] + CharacterName[TrainerOrder] + "!", 38, GrayTextColor);
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
                    gTarget.render(Position_of_Target[IndexOfTarget][0], Position_of_Target[IndexOfTarget][1], &Target);
                }
                else if(countText == 8)
                {
                    loadText(gLectureSpeech, LectureSpeech[countText] + PokemonName[IndexOfTarget], 38, GrayTextColor);
                }
                else if(countText >= 10)
                {
                    skip = true;
                }
                else loadText(gLectureSpeech, LectureSpeech[countText], 38, GrayTextColor);
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
                            Mix_PlayChannel(-1, gMenuChooseEffect, 0);
                            break;
                        case SDLK_e:
                            skip = true;
                            Mix_PlayChannel(-1, gMenuChooseEffect, 0);
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
            Mix_HaltMusic();
            Mix_PlayMusic(gIngameMusic, -1);
            loadCharacter(TrainerOrder);
            currentRect = SpriteClips[0][0];
            SDL_RenderClear(gRenderer);
            while(!quit)
            {
                capTimer.start();
                SDL_RenderClear(gRenderer);
                gPlayGround.render(0, 0, &camera);

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

                        switch(e.key.keysym.sym)
                        {
                            case SDLK_RIGHT:
                            {
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_RIGHT][frameTurn[Move%4]];
                                Move++;
                                //check encounter NPC:
                                if(checkNPCsColision(e))
                                {
                                    NPCDirection = KEY_PRESS_SURFACE_LEFT;
                                    Move = 0;
                                    encouterNPC = true;
                                    break;
                                }

                                if(X_Position_of_Character <= (ScreenWidth/2) - CharacterWidth || camera.x + VelX > ScreenWidth)
                                {
                                    X_Position_of_Character += VelX;
                                }
                                else
                                {
                                    camera.x += VelX;
                                    Position_of_NPC[0][0] -= VelX;
                                    Position_of_NPC[1][0] -= VelX;
                                    Position_of_NPC[2][0] -= VelX;
                                }
                                interactNPC = false;

                                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                                break;
                            }
                            case SDLK_LEFT:
                            {
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_LEFT][frameTurn[Move%4]];
                                Move++;
                                if(checkNPCsColision(e))
                                {
                                    NPCDirection = KEY_PRESS_SURFACE_RIGHT;
                                    Move = 0;
                                    encouterNPC = true;
                                    break;
                                }
                                if(X_Position_of_Character >= (ScreenWidth/2) - CharacterWidth || camera.x - VelX < 0)
                                {
                                    X_Position_of_Character -= VelX;
                                }
                                else
                                {
                                    camera.x -= VelX;
                                    Position_of_NPC[0][0] += VelX;
                                    Position_of_NPC[1][0] += VelX;
                                    Position_of_NPC[2][0] += VelX;
                                }
                                interactNPC = false;

                                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                                break;
                            }
                            case SDLK_UP:
                            {
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_UP][frameTurn[Move%4]];
                                Move++;
                                if(checkNPCsColision(e))
                                {
                                    NPCDirection = KEY_PRESS_SURFACE_DOWN;
                                    encouterNPC = true;
                                    Move = 0;
                                    break;
                                }
                                if(Y_Position_of_Character >= (ScreenHeight/2) - CharacterHeight || camera.y - VelY < 0)
                                {
                                    Y_Position_of_Character -= VelY;
                                }
                                else
                                {
                                    camera.y -= VelY;
                                    Position_of_NPC[0][1] += VelY;
                                    Position_of_NPC[1][1] += VelY;
                                    Position_of_NPC[2][1] += VelY;
                                }
                                interactNPC = false;

                                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                                else Mix_PlayChannel(-1, gFootStepEffect, 0);

                                break;
                            }
                            case SDLK_DOWN:
                            {
                                currentRect = SpriteClips[KEY_PRESS_SURFACE_DOWN][frameTurn[Move%4]];
                                Move++;
                                if(checkNPCsColision(e))
                                {
                                    NPCDirection = KEY_PRESS_SURFACE_UP;
                                    encouterNPC = true;
                                    Move = 0;
                                    break;
                                }
                                if(Y_Position_of_Character <= (ScreenHeight/2) - CharacterHeight || camera.y + VelY > ScreenHeight)
                                {
                                    Y_Position_of_Character += VelY;
                                }
                                else
                                {
                                    camera.y += VelY;
                                    Position_of_NPC[0][1] -= VelY;
                                    Position_of_NPC[1][1] -= VelY;
                                    Position_of_NPC[2][1] -= VelY;
                                }
                                interactNPC = false;

                                if(camera.y >= 320) Mix_PlayChannel(-1, gFootStepBushEffect, 0);
                                else Mix_PlayChannel(-1, gFootStepEffect, 0);
                                break;
                            }
                            case SDLK_SPACE:
                            {
                                if(encouterNPC)
                                {
                                     interactNPC = true;
                                }
                            }
                        }
                    }
                }

                float avgFPS = countFrame/ (fpsTimer.getTicks() /1000.f);
                if(avgFPS > 2000000) avgFPS = 0;

                gNPC.render(Position_of_NPC[0][0], Position_of_NPC[0][1], &NPC[NPCDirection][0]);
                gNPC.render(Position_of_NPC[1][0], Position_of_NPC[1][1], &NPC[NPCDirection][1]);
                gNPC.render(Position_of_NPC[2][0], Position_of_NPC[2][1], &NPC[NPCDirection][2]);
                gSpriteSheetTexture.render(X_Position_of_Character, Y_Position_of_Character, &currentRect);
                gScoreBoard.render(0, 0, &ScoreBoard);
                loadText(gPLayerScore, to_string(PlayerScore), 12, WhiteTextColor);
                loadText(gPLayerPokeball, to_string(PlayerPokemonBall), 12, WhiteTextColor);
                gPLayerScore.render(36, 8);
                gPLayerPokeball.render(36, 32);
                if(interactNPC)
                {
                    gTextBox.render(60, 380, &TextBox);

                    if(meetPro)
                    {
                        int randTips = generateRandomNumber()%2;
                        loadText(gNPCsSpeech, ProfSpeech[randTips], 38, GrayTextColor);
                    }
                    else if(meetNurse)
                    {
                        PlayerHealthBar.w = HealthBarWidth;
                        loadText(gNPCsSpeech, NurseSpeech[0], 38, GrayTextColor);
                    }
                    else if(meetSeller)
                    {
                        if(PlayerScore < 100) loadText(gNPCsSpeech, SellerSpeech[0], 38, GrayTextColor);
                        else
                        {
                            PlayerScore -= 100;
                            loadText(gNPCsSpeech, SellerSpeech[1], 38, GrayTextColor);
                        }
                    }
                    gNPCsSpeech.render(90, 390);
                }
                int rate = generateRandomNumber()%10;
                int randomPokemon = generateRandomNumber()%TOTAL_POKEMON;
                int FightingTextIndex = 0;
                int BonusPoint = 20;
                while(camera.y >= 320  && (rate == 1)  && !run)
                {
                    //Loop for combat

                    Mix_PauseMusic();
                    Mix_PlayChannel(-1, gBattleMusic, -1);
                    WildPokemonLevel = generateRandomNumber()%(PlayerPokemonLevel + 1) + 1;
                    while(!quit)
                    {

                        float catchSuccessfullyRate = 100*(((float)HealthBarWidth - (float)WildHealthBar.w)/(float)HealthBarWidth);

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
                                    case SDLK_DOWN:
                                    {
                                        X_Position_of_Poiter = Position_of_Pointer[2][0];
                                        Y_Position_of_Pointer = Position_of_Pointer[2][1];
                                        break;
                                    }
                                    case SDLK_UP:
                                    {
                                        X_Position_of_Poiter = Position_of_Pointer[0][0];
                                        Y_Position_of_Pointer = Position_of_Pointer[0][1];
                                        break;
                                    }
                                    case SDLK_SPACE:
                                    {
                                        if(X_Position_of_Poiter == Position_of_Pointer[0][0] )
                                        {
                                            if(Y_Position_of_Pointer == Position_of_Pointer[0][1])
                                            {
                                                fight = true;
                                                run = false;
                                                FightingTextIndex = PLAYER_CHOOSE_FIGHT;
                                            }
                                            else
                                            {
                                                pressCatch = true;
                                                run = false;
                                            }
                                        }
                                        else
                                        {
                                            run = true;
                                            FightingTextIndex = 1;
                                        }
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
                        currentWildPokemon = pokemonFighting[randomPokemon][FrontOfPokemon];
                        currentCharacterPokemon = pokemonFighting[IndexOfTarget][BackOfPokemon];
                        gPokemonFighting.render(430, 100, &currentWildPokemon);
                        gPokemonFighting.render(90, 240, &currentCharacterPokemon);
                        gWildHealthBar.render(143, 90, &WildHealthBar);
                        gPlayerHealthBar.render(477, 300, &PlayerHealthBar);

                        //FIRST_ENCOUNTER_TEXT,//0
                        //WAIT_FOR_PLAYERMOVE,//1
                        //PLAYER_CHOOSE_FIGHT,//2
                        //PLAYER_WIN, //3
                        //PLAYER_LOSE, //4
                        //PLAYER_CAUGHT, //5
                        //PLAYER_MISSED, //6

                        if(!run)
                        {
                            if(fight)
                            {
                                WildHealthBar.w -= PlayerPokemonLevel*6;
                                PlayerHealthBar.w -= WildPokemonLevel*6;
                            }
                            if(pressCatch && PlayerPokemonBall > 0)
                            {
                                int catchRate = generateRandomNumber()%100 + 1;
                                if(catchRate < catchSuccessfullyRate)
                                {
                                    FightingTextIndex = PLAYER_CAUGHT;
                                    PlayerPokemonBall--;
                                    PlayerScore += WildPokemonLevel*5 + BonusPoint*(WildPokemonLevel > PlayerPokemonLevel);
                                    run = true;
                                }
                                else
                                {
                                    FightingTextIndex = PLAYER_MISSED;
                                    PlayerHealthBar.w -= WildPokemonLevel*6;
                                }
                                pressCatch = false;

                            }
                            else if(pressCatch && PlayerPokemonBall <= 0)
                            {
                                FightingTextIndex = PLAYER_OUT_OF_BALL;
                                pressCatch = false;
                            }
                            fight = false;
                        }

                        if(WildHealthBar.w < 0 || PlayerHealthBar.w < 0)
                        {
                            if(WildHealthBar.w < 0 && PlayerHealthBar.w > 0)
                            {
                                FightingTextIndex = PLAYER_WIN;
                                PlayerPokemonLevel++;
                                PlayerScore += WildPokemonLevel*2 + BonusPoint*(WildPokemonLevel > WildPokemonLevel);
                            }
                            else
                            {
                                FightingTextIndex = PLAYER_LOSE;
                                PlayerScore -= (PlayerPokemonLevel - WildPokemonLevel + 2)*2;
                            }
                            run = true;
                        }
                        if(FightingTextIndex == FIRST_ENCOUNTER_TEXT || FightingTextIndex == PLAYER_CAUGHT) loadText(gFightingSpeech, PokemonEncounterText[FightingTextIndex] + PokemonName[randomPokemon], 38, GrayTextColor);
                        else loadText(gFightingSpeech, PokemonEncounterText[FightingTextIndex], 38, GrayTextColor);

                        loadText(gWildLevel,"Lv " + to_string(WildPokemonLevel), 38, GrayTextColor);
                        loadText(gPLayerLevel, "Lv "  + to_string(PlayerPokemonLevel), 38, GrayTextColor);
                        gFightingSpeech.render(15, 380);
                        gPLayerLevel.render(570, 258);
                        gWildLevel.render(240, 48);
                        SDL_RenderPresent(gRenderer);
                        if(run || quit)
                        {
                            SDL_Delay(1000);
                            break;
                        }

                        ///Cap FPS
                        countFrame++;
                        int frameTicks = capTimer.getTicks();
                        if(frameTicks < ScreenTickPerFrame)
                        {
                            SDL_Delay(ScreenTickPerFrame - frameTicks);
                        }
                    }
                    if(run || quit)
                    {
                        Mix_HaltChannel(-1);
                        Mix_ResumeMusic();
                    }
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

    }
    close();

    return 0;
}
