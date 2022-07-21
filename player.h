/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  player.h
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
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

#include "ship.h"


class player: public ship {
    public:
        player();
        ~player();

        void enterPlayer();
        void monitorPlayer();
        
    protected:
        

    private:
        void rotatePlayer();
        void movePlayer();
};


#endif