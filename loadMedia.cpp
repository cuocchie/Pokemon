#include "loadMedia.h"

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
               if(j == 0) pokemonFighting[i][j] = {j*90, i*120, 90, 120};
               if(j == 1) pokemonFighting[i][j] = {j*90, i*120, 130, 120};
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
                NPC[i][j] = {j*CharacterWidth, i*CharacterHeight, CharacterWidth, CharacterHeight};
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
        combatBox = {0, 0, ScreenWidth, ScreenHeight};
    }
    if(!gPointer.loadFromFile("data/Pointer.png"))
    {
        cout << "Failed to load Pointer.png\n";
        success = false;
    }
    else
    {
        Pointer = {0, 0, 40 , 40};
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
    gTackleSound = Mix_LoadWAV("Sound/SFX Tackle.mp3");
    if(gTackleSound == NULL)
    {
        cout << "Failed to load Tackle SFX! SDL_mixer error: " << Mix_GetError() << endl;
    }
    gLoseSoundEffect = Mix_LoadWAV("Sound/SFX Lose.mp3");
    if(gLoseSoundEffect == NULL)
    {
        cout << "Failed to load LoseSoundEffect! SDL_mixer error: " << Mix_GetError() << endl;
    }
    gLevelUp = Mix_LoadWAV("Sound/SFX Level Up.mp3");
    if(gLevelUp == NULL)
    {
        cout << "Failed to load SFX Level Up! SDL_mixer error: " << Mix_GetError() << endl;
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
