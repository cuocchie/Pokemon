#include "globalVar.h"
#include "SDLclass.h"
#include "CheckGameEvent.h"
#include "PokemonInfo.h"
#include "loadMedia.h"
#include "loadMenuScreen.h"
#include "loadChooseTrainerScreen.h"
#include "loadGameLectureScreen.h"
#include "loadFightingGameScreen.h"
#include "loadMainGameScreen.h"
#include "closeSDL.h"
using namespace std;

int X_Position_of_Character = (ScreenWidth/2) - CharacterWidth, Y_Position_of_Character = (ScreenHeight/2) - CharacterHeight;
int Position_of_Pointer[3][2] = {{345, 375}, {485, 375}, {345, 420}};
int Position_of_Target[3][2] = {{40, 80}, {90, 175}, {140, 80}};
int Position_of_NPC[NumberOfNPCs][2] = {{100, 100}, {640, 480}, {1050, 100}};
int TrainerOrder = 0;
int IndexOfTarget = 0;
int randTips;
SDL_Event e;
bool meetPro = false, meetNurse = false, meetSeller = false;
int X_Position_of_Poiter = Position_of_Pointer[0][0], Y_Position_of_Pointer = Position_of_Pointer[0][1];
int HealthBarWidth = 144;
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
Mix_Chunk *gTackleSound = NULL;
Mix_Chunk *gLoseSoundEffect = NULL;
Mix_Chunk *gLevelUp = NULL;

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
SDL_Rect currentRect;
SDL_Rect currentCharacterPokemon;
SDL_Rect camera = {0, 0, ScreenWidth, ScreenHeight};
SDL_Rect currentWildPokemon = {420, 90, 120, 120};

//color
SDL_Color GrayTextColor = {65, 64, 63};
SDL_Color ShadowTextColor = {215, 214, 213};
SDL_Color WhiteTextColor = {255, 255, 255};

CheckGameEvent GameChecking;
//rendered texture

///text:
LTexture gPLayerLevel, gWildLevel;
LTexture gPLayerScore, gPLayerPokeball;
LTexture gLectureSpeech;
LTexture gNPCsSpeech;
LTexture gFightingSpeech;
LTexture gAskSwitch;
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
PokemonBaseInfo everyPokemon[TOTAL_POKEMON] = {{0, PokemonName[0], 49, 49, 45}, {1, PokemonName[1], 52, 43, 39}, {2, PokemonName[2], 48, 65, 44}, {3, PokemonName[3], 45, 40, 40}, {4, PokemonName[4], 56, 35, 30}, {5, PokemonName[5], 35, 55 ,40}, {6, PokemonName[6], 20, 45, 100}, {7, PokemonName[7], 50, 95, 50}, {8, PokemonName[8], 80, 100, 40}, {9, PokemonName[9], 100, 100, 100}};

int main(int argc, char* argv[])
{

    srand(time(0));
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

            int TrainerOrder = 0;
            int NPCDirection = KEY_PRESS_SURFACE_DOWN;
            int PlayerScore = 100, PlayerPokemonBall = 10;
            int countFrame = 0, Move = 0;
            LTimer fpsTimer;
            LTimer capTimer;

            //Start Game Menu:
            fpsTimer.start();
            //Game Loop
            loadGameMenuScreen(capTimer, countFrame);
            loadGameChoosingPlayer(capTimer, countFrame);
            loadGameLectureScreen(capTimer, countFrame);
            PokemonInGameInfo PlayerPokemon(IndexOfTarget, 5);
            loadMainGameScreen(capTimer, PlayerPokemon, Move, NPCDirection, countFrame, fpsTimer);
        }

    }
    close();

    return 0;
}
