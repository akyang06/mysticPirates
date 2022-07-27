/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  enemyRed.h
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function definitions of enemy red class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __ENEMYRED_H__
#define __ENEMYRED_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"

#include "ship.h"
#include "enemy.h"


class enemyRed: public enemy {
    public:
        enemyRed();
        ~enemyRed();

        void monitorEnemyRed(player &p1);

    protected:
        

    private:
        void moveEnemyRed();
        void getInRange();
        void trajectoryInRange();
        void rotateToGetInRange(float upperAngle, float lowerAngle);
        void attackPlayer();
        void rotateToAttackPlayer(float shootingAngle);
        void circleAround();
        void monitorCoolDown();

        bool aboveOrBelowPlayer();

        bool shotFired;
        bool circleAroundCC;

        float cooldownDuration;
        float cooldown;

        
};


#endif