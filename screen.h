/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  screen.h
 *  Authors: Alex Savic & Audrey Yang
 *  06/28/22
 *
 *  
 *
 *  Function definitions of screen class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"

#include "player.h"
#include "enemy.h"

class screen {
    public:
        screen();
        ~screen();

        void titleScreen();

    private:
       
        void tutorialScreen();

        Rectangle drawButton(const char *text, float roundness, int segments, Font pirateFont, int fontSize, int screenWidth, int screenHeight, float divisor);

        void unloadItems(Texture2D currTexture, Font currFont, Music currMusic);

        int screenWidth;
        int screenHeight;
};


#endif