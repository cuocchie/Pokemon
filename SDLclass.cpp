#include "SDLclass.h"
#include <bits/stdc++.h>
#include <SDL.h>
#include "globalVar.h"
///Ltimer
LTimer::LTimer()
{
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTicks = SDL_GetTicks(); //Get the current time
    mPausedTicks = 0;
}

void LTimer::stop()
{
    mStarted = false;
    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}
void LTimer::pause()
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

void LTimer::unpause()
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

Uint32 LTimer::getTicks()
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


bool LTimer::isStarted()
{
    return mStarted;
}

bool LTimer::isPaused()
{
    return mPaused && mStarted;
}
///LTexture

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}
//Deallocates memory
LTexture::~LTexture()
{
    free();
}
//Loads image at specified path
bool LTexture::loadFromFile(string path )
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

bool LTexture::loadFromRenderedText(string textureText, SDL_Color textColor, int Wraped)
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
void LTexture::free()
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
void LTexture::render( int x, int y, SDL_Rect* clip)
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
int LTexture::getWidth()
{
    return mWidth;
}
int LTexture::getHeight()
{
    return mHeight;
}



