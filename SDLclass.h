#ifndef SDLCLASS
#define SDLCLASS

#include <bits/stdc++.h>
#include <SDL.h>
#include "globalVar.h"
using namespace std;
class LTimer
{
    public:
		//Initializes variables
		LTimer();
		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();
		//Gets the timer's time
		Uint32 getTicks();
		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

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
    LTexture();
    //Deallocates memory
    ~LTexture();
    //Loads image at specified path
    bool loadFromFile( std::string path );
    bool loadFromRenderedText(string textureText, SDL_Color textColor, int Wraped);
    //Deallocates texture
    void free();
    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL);
    //Gets image dimensions
    int getWidth();
    int getHeight();
private:
    SDL_Texture* mTexture;
    //Image dimensions;
    int mWidth;
    int mHeight;

};

#endif // SDLCLASS
