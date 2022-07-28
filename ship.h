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

#define MAX_SHOTS   10

typedef struct Cannonballs {
    Vector2 position;
    Vector2 speed;
    float radius;
    float rot;
    int lifeSpawn;
    bool active;
    Color color;
} Cannonballs;


class ship {
    public:
        ship();
        ~ship();

        void drawShip();
        int getX();
        int getY();
        float getRotation();
        Cannonballs* getCannonballs();

        bool enteredBounds;
        
    protected:

        bool outOfBounds();
        bool facingInBounds();
        bool inCorner();

        void boundCollision();

        void accelerateShip(float amount);
        void decelerateShip(float amount);

        void sideCannonAttack();
        void frontCannonAttack();
        void fireBarrelAttack();
        
        void monitorCanonballs();
        void monitorFirebarrel();

        void checkCollision();
        void monitorCoolDown();
        
        int screenWidth;
        int screenHeight;

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
        float rotationSpeed;
        int range;

        Texture2D shipTexture;

        Rectangle sourceRec;
        Rectangle destRec;
        Rectangle targetRec;
        Vector2 origin;  

        float upBounds;
        float downBounds;
        float leftBounds;
        float rightBounds;

        int healthBar;
        int shootType;
        bool isAlive;

        bool frontCannonUnlocked;
        bool sideCannonsUnlocked;
        bool fireBarrelUnlocked;

        bool frontCannonAvailable;
        bool sideCannonsAvailable;
        bool fireBarrelAvailable;

        float frontCannonCooldownDuration;
        float frontCannonCooldown;

        float sideCannonsCooldownDuration;
        float sideCannonsCooldown;

        float fireBarrelCooldownDuration;
        float fireBarrelCooldown;
        
    private:
        float collisionDrag;
        
};


#endif