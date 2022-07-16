/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  player.h
 *  Authors: Alex Savic & Audrey Yang
 *  06/28/22
 *
 *  
 *
 *  Function definitions of player class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"


class player {
    public:
        player(int screenWidth, int screenHeight);
        ~player();

        void drawPlayer();
        void monitorPlayer();

    private:
        void movePlayer();

        int screenWidth;
        int screenHeight;

        int playerWidth;
        int playerHeight;

        float velMag;

        float rotation;

        bool right, left, up, down;

        Texture2D playerTexture;

        Rectangle sourceRec;
        Rectangle destRec;
        Vector2 origin;  
};


#endif