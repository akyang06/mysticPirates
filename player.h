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
        void monitorPlayer(std::vector<ship*> &allShips);
        void unloadPlayerComponents();
        
    protected:
        

    private:
        int screenWidth;
        int screenHeight;
        void rotatePlayer();
        void movePlayer();
        void playerAttack();
        void attackType();
        void monitorPlayerAttack();
};


#endif