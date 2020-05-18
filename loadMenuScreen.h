#ifndef LOADMENUSCREEN_H_INCLUDED
#define LOADMENUSCREEN_H_INCLUDED

#include "globalVar.h"
#include "SDLclass.h"
#include "CheckGameEvent.h"
#include "loadMedia.h"

void ChooseMenu();
void ShowMenuScreen();
void loadGameMenuScreen(LTimer& capTimer, int& countFrame);

#endif // LOADMENUSCREEN_H_INCLUDED
