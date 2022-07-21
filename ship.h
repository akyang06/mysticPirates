/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  ship.h
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function definitions of ship class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SHIP_H__
#define __SHIP_H__

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

#include "raylib.h"
#include "rlgl.h"


class ship {
    public:
        ship(int screenWidth, int screenHeight);
        ~ship();

        void drawShip();
        int getX();
        int getY();
        
    protected:

        bool outOfBounds();
        bool facingInBounds();

        void boundCollision();
        bool inCorner();

        void accelerateShip(float amount);
        void decelerateShip(float amount);

        void playerShoot();

        int shipWidth;
        int shipHeight;

        float velMag;
        float rotation;

        float velLimit;
        Vector2 velComp;

        float acceration;
        float deceleration;
        float turnDrag;
        float drag;

        Texture2D shipTexture;

        Rectangle sourceRec;
        Rectangle destRec;
        Vector2 origin;  

    private:
        int screenWidth;
        int screenHeight;

        float collisionDrag;

        float upBounds;
        float downBounds;
        float leftBounds;
        float rightBounds;

};


#endif