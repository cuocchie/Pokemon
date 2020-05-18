#ifndef GLOBAL_VARIABLES
#define GLOBAL_VARIABLES

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
const string PokemonName[] = {"Bulbasaur", "Charmander", "Squirtle","Pidgey", "Rattata", "Pikachu", "Snorlax", "Cubone", "Geodude", "Mew"};
const string LectureSpeech[] = {"Press SPACE to continue or press E to\nskip instruction.","Hello ", "My name is Professor Oak\nWelcome to the Pokemon World!!", "Before head into Pokemon world,\nlet me show you something", "As being a trainer, your ambition is\nbeing one of the best Pokemon trainer.", "To do that, you have to defeat or catch\nPokemon as much as you can.", "You will encounter wild Pokemon if\nwalking in the bush area.", "So, take care.","But, you need a Pokemon first.", "Now choose your Pokemon.", "You choose a ", "Here, take 100 Point and 10 Pokeballs\nas my present", "And remember, if you need help,\nmeet Prof.Elm on the left of the map." ,"Okay, now if you are ready press SPACE\nto enter the Pokemon world. Good luck!"};
const string ProfSpeech[] = {"Hi, I am Professor Elm!", "You are new here.", "Do you know that there is a myth?", "You can buy more PokeBall from a black\nsuit guy.", "You should heal your Pokemon after a\nfight"};
const string NurseSpeech[] = {"Your Pokemon is fully healed"};
const string SellerSpeech[] = {"Press SPACE to buy 10 pokeballs for\n100 Pokedollars"};
const string PokemonEncounterText[] = {"You encountered a wild\n", "You've run away", "Your Pokemon used Tackle", "Wild Pokemon used Tackle", "Your Pokemon wins\nYour Pokemon's now level ", "Your Pokemon Loses", "You caught a wild\n", "Missed", "You are out of Ball", "What are we gonna do?"};
const int BonusPoint = 20;
const int Direction = 2, TOTAL_POKEMON = 10, FrontOfPokemon = 0, BackOfPokemon = 1;

extern int X_Position_of_Character, Y_Position_of_Character;
extern int Position_of_Pointer[3][2];
extern int Position_of_Target[3][2];
extern int Position_of_NPC[NumberOfNPCs][2];
extern int TrainerOrder;
extern int IndexOfTarget;
extern int randTips;
extern SDL_Event e;
extern bool meetPro, meetNurse, meetSeller;
extern int X_Position_of_Poiter, Y_Position_of_Pointer;
extern int HealthBarWidth;
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
    PLAYER_RAN,//1
    PLAYER_FIGHT,//2
    WILD_FIGHT, //3
    PLAYER_WIN, //4
    PLAYER_LOSE, //5
    PLAYER_CAUGHT, //6
    PLAYER_MISSED, //7
    PLAYER_OUT_OF_BALL,//8
    WAIT_FOR_MOVE//9
};
enum NPCs
{
    ProfessorElm,
    Nurse,
    Seller,
};
//SDL variable
//load individual image
extern SDL_Surface* loadSurface(string path);

extern SDL_Window* gWindow;

extern SDL_Texture* loadTexture(string path);
//Current displayed texture
extern SDL_Texture* gCurrentSurface;
//The window renderer
extern SDL_Renderer* gRenderer;
//Rendered texture
extern TTF_Font* gFont;

//The music that will be played
extern Mix_Music *gMenuMusic;
extern Mix_Music *gLectureMusic;
extern Mix_Music *gIngameMusic;
extern Mix_Chunk *gBattleMusic;
//The sound effects that will be used
extern Mix_Chunk *gMenuEffect;
extern Mix_Chunk *gMenuChooseEffect;
extern Mix_Chunk *gFootStepEffect;
extern Mix_Chunk *gFootStepBushEffect;
extern Mix_Chunk *gTackleSound;
extern Mix_Chunk *gLoseSoundEffect;
extern Mix_Chunk *gLevelUp;

extern SDL_Rect MenuScreen, MenuOptions[2][2];
extern SDL_Rect PokemonBanner;
extern SDL_Rect TrainerScreen, Trainer[6];
extern SDL_Rect SpriteClips[KEY_PRESS_SURFACE_TOTAL][TOTAL_FRAME];
extern SDL_Rect NPC[KEY_PRESS_SURFACE_TOTAL][3];
extern SDL_Rect pokemonFighting[TOTAL_POKEMON][Direction];
extern SDL_Rect ScoreBoard;
extern SDL_Rect LectureBackground;
extern SDL_Rect LectureProf;
extern SDL_Rect Target;
extern SDL_Rect playGround;
extern SDL_Rect TextBox;
extern SDL_Rect combatBox;
extern SDL_Rect Pointer;
extern SDL_Rect PlayerHealthBar, WildHealthBar;
extern SDL_Rect currentRect;
extern SDL_Rect currentCharacterPokemon;
extern SDL_Rect camera;
extern SDL_Rect currentWildPokemon;

//color
extern SDL_Color GrayTextColor;
extern SDL_Color ShadowTextColor;
extern SDL_Color WhiteTextColor;


#endif // GLOBAL_VARIABLES
