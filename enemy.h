/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  enemy.h
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function definitions of enemy class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"

#include "ship.h"
#include "player.h"


class enemy: public ship {
    public:
        enemy();
        ~enemy();

        
    protected:

        void facePlayer();
        void moveEnemyInBounds();

        int range;
        Vector2 distToPlayer;
        float distMag;
        
    private:
           
};


#endif