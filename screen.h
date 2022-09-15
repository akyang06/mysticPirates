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

#include "ship.h"
#include "player.h"
#include "enemy.h"
#include "enemyRed.h"

class screen {
    public:
        screen();
        ~screen();

        void titleScreen();

    private:
        void tutorialScreen();
        void tortugaHubScreen();
        void challengeScreen();
        void marketScreen();
        void victoryScreen(int enemyOneSpawnTypes[5], int enemyTwoSpawnTypes[5]);
        void defeatScreen();
        void aboutUsScreen();

        Rectangle drawButton(const char *text, float roundness, int segments, Font pirateFont, int fontSize, int screenWidth, int screenHeight, float divisor);

        Texture2D drawImages(Image img, int width, int height);

        void drawPopUpWindow();
        void progressPopUp();
        void instructionsPage(Texture2D instructionsTexture);
        void drawFuturePopUp();

        int screenWidth;
        int screenHeight;

        bool exitPopUp;
        bool toHub;
        bool instructionsPopUp;
        bool futurePopUp;

        bool playerHealth;
        bool enemyOneHealth;
        bool enemyTwoHealth;
};


#endif