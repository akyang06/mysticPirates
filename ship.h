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

#define MAX_SHOTS   10
#define EXPLOSION_RADIUS 300

typedef struct Shoot {
    Vector2 position;
    Vector2 speed;
    float radius;
    float rot;
    int lifeSpawn;
    bool active;
} Shoot;

typedef struct Firebarrel {
    Vector2 position;
    float explosionTimer;
    int frame;
    bool active;
    bool exploded;
} Firebarrel;

typedef struct shipCollisionStruct {
    Vector2 collidingEdge;
    float separation;
} shipCollisionStruct;

class ship {
    public:
        ship();
        ~ship();

        void drawShip();
        int getX();
        int getY();
        float getRotation();

        bool enteredBounds;

        Rectangle loot;
        bool spawnLoot;
        bool lootPickedUp;

    protected:

        bool outOfBounds();
        bool facingInBounds();
        bool inCorner();

        void boundCollision();

        void accelerateShip(float amount);
        void decelerateShip(float amount);
        void updateVelComp();

        void sideCannonAttack();
        void frontCannonAttack();
        void fireBarrelAttack();
        
        void monitorCanonballs();
        void monitorFirebarrel();
        void drawBarrelExplosion(int barrelIndex);

        int lootDrop();

        void monitorCollisions();
        void monitorShipToWeaponCollisions();
        void monitorShiptoShipCollisions();
        void monitorCoolDown();

        void isShipToShipColliding(ship* otherShip);
        shipCollisionStruct findMinSeparation(ship* shipA, ship* shipB);
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

        float acceleration;
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
        shipCollisionStruct collision;

        float upBounds;
        float downBounds;
        float leftBounds;
        float rightBounds;

        bool edgeCollision;
        bool shipCollision;

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

        /* Loot (replace colors with images later) */
        void shipStatus();
        int spawnTypes[5];
        int lootExpire;
        int trackLoot;

        bool hasCollided;

        std::vector<ship*> allShips;
        std::vector<Shoot> shoot;
        std::vector<Firebarrel> barrel;
        std::vector<Vector2> hitBoxVertices;
        std::vector<Vector2> hitBoxEdges;
        
    private:
        float collisionDrag;
        Texture2D explosion;
        Sound firebarrelExplosionSound;
};


#endif