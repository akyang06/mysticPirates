/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  play.h
 *  Authors: Alex Savic & Audrey Yang
 *  07/8/22
 *
 *  
 *
 *  Function definitions of play class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __PLAY_H__
#define __PLAY_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"

class play {
    public:
        play(int screenWidth, int screenHeight);
        ~play();

        void playScreen();


    private:
        int screenW;
        int screenH;

};


#endif