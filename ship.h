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
#include <time.h>
#include <vector>
#include <limits>

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

typedef struct shipCollision {
    Vector2 collidingEdge;
    float separation;
} shipCollision;


class ship {
    public:
        ship();
        ~ship();

        void drawShip();
        int getX();
        int getY();
        float getRotation();

        bool enteredBounds;

        //void unloadComponents();
        Rectangle dropPoint;
        
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
        void drawExplosion();

        void checkCollision();
        void monitorCoolDown();

        Color lootDrop();
        void lootPickup();
        void monitorShipCollisions();
        void isShipToShipColliding(ship* otherShip);
        shipCollision findMinSeparation(ship* shipA, ship* shipB);
        void shipToShipCollision(Vector2 collidingEdge);
        void computeHitBox();
        
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
        Texture2D barrelTexture;

        Rectangle sourceRec;
        Rectangle destRec;
        Rectangle hitBox;
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

        bool targetRecAlive;

        /* Loot (replace colors with images later) */
        bool lootSpawn;
        int lootTypeInt;
        Color lootTypeColor;
        bool drawLoot;

        bool hasCollided;

        std::vector<ship*> allShips;

        std::vector<Vector2> hitBoxVertices;
        std::vector<Vector2> hitBoxEdges;
        
    private:
        float collisionDrag;
};


#endif