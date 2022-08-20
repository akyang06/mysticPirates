/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  ship.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function implementations of ship class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <random>
#include <ctime> 
#include "ship.h"

#define MAX_SHOTS   10
#define EXPLOSION_RADIUS 50

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
} Firebarrel;

static Shoot shoot[MAX_SHOTS] = { 0 };
static Firebarrel barrel[MAX_SHOTS] = { 0 };
 
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: constructor
 * @purpose: Initializes a ship object and loads in the base image of a ship with correct dimensions
 *
 * @parameters: the screen width and height
 *     
 * @returns: nothing
 * @effects: sets the values of shipWidth and shipHeight, also defines the shipTexture
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ship::ship() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    /* Sets up universal values */
    rotation = 0.0;
    velMag = 0.0;
    velComp = (Vector2){0, 0};
    collisionDrag = 0.01;
    enteredBounds = false;
    healthBar = 100;
    isAlive = true;
    

    /* Initializes the shooting component */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        shoot[i].position = (Vector2){0, 0};
        shoot[i].speed = (Vector2){0, 0};
        shoot[i].radius = 5;
        shoot[i].active = false;
        shoot[i].lifeSpawn = 0;
    }

    /* Sets up hitBox vector sizes */
    hitBoxVertices.resize(4);
    hitBoxEdges.resize(4);

    /* Initializes the attack type to cannon */
    shootType = 1;

    /* Target rectangle initialized as true */
    targetRecAlive = true;
    spawnLoot = true;
    lootPickedUp = false;

    /* Initializes collided status of ship */
    hasCollided = false;

    /* Initializes the different loot pickups to 0 */
    for (int i = 0; i < 5; i++) {
        spawnTypes[i] = 0;
    }
    //lootTypeStr = "";
    lootExpire = 750.0f;

    /* Initializes collided status of ship */
    edgeCollision = false;
    shipCollision = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: destructor
 * @purpose: Deallocates all heap allocated memory
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ship::~ship() {
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawShip
 * @purpose: Draws ship on the screen
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Updates the destRec x and y components based on velocity and rotation
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::drawShip() {
    destRec.x += velComp.x;
    destRec.y += velComp.y;

    if (targetRecAlive) {
        hitBox.x = destRec.x - destRec.width/2;
        hitBox.y = destRec.y - destRec.height/2;
    }

    computeHitBox();

    /* Draws ship on screen
    Note: rotation is multiplied by FPS to compensate for the BeginDrawing function */
    if (isAlive) {
        DrawTexturePro(shipTexture, sourceRec, destRec, origin, rotation * 57.3, (Color){255,255,255,255});

        /* Health bar status */
        DrawRectangle(hitBox.x - 7, hitBox.y - 17, healthBar, 7, (Color){ 0, 228, 48, 255 });

        // /* Draws the hitbox of the ship */
        // DrawLineV(hitBoxVertices.at(0), hitBoxVertices.at(1), (Color){0,0,0,255});
        // DrawLineV(hitBoxVertices.at(1), hitBoxVertices.at(2), (Color){0,0,0,255});
        // DrawLineV(hitBoxVertices.at(2), hitBoxVertices.at(3), (Color){0,0,0,255});
        // DrawLineV(hitBoxVertices.at(3), hitBoxVertices.at(0), (Color){0,0,0,255});
    }

    DrawText(TextFormat("red: %d", spawnTypes[0]), 20, 50, 20, (Color){255,255,255,255});
    DrawText(TextFormat("orange: %d", spawnTypes[1]), 20, 80, 20, (Color){255,255,255,255});
    DrawText(TextFormat("yellow: %d", spawnTypes[2]), 20, 110, 20, (Color){255,255,255,255});
    DrawText(TextFormat("blue: %d", spawnTypes[3]), 20, 140, 20, (Color){255,255,255,255});
    DrawText(TextFormat("purple: %d", spawnTypes[4]), 20, 170, 20, (Color){255,255,255,255});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: getX
 * @purpose: Determines the x position of the ship
 *
 * @parameters: none
 *     
 * @returns: the x positon of the ship
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int ship::getX() {
    return destRec.x;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: getY
 * @purpose: Determines the y position of the ship
 *
 * @parameters: none
 *     
 * @returns: the y positon of the ship
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int ship::getY() {
    return destRec.y;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: getRotation
 * @purpose: Returns the rotation value of the ship
 *
 * @parameters: none
 *     
 * @returns: the rotation of the ship
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
float ship::getRotation() {
    return rotation;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: accelerateEnemy
 * @purpose: Increases the speed of the ship until it reaches it's velocity limit
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: velMag of ship increases
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::accelerateShip(float amount) {
    if(velMag < velLimit - amount) {
        velMag += amount;
    }
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: decelerateShip
 * @purpose: Decreases the speed of the enemy until it reaches 0
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: velMag of enemy decreases
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::decelerateShip(float amount) {
    if(velMag > 0) {
        velMag -= amount;
    }  
    
    if (velMag < 0) {
        velMag = 0;
    }
}

void ship::updateVelComp() {
    velComp = (Vector2) {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorCollisions
 * @purpose: Checks collisions with other ships, bounds and attacks
 *
 * @parameters: none
 *     
 * @returns: none
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorCollisions() {

    /* Boundary collisions */
    if (outOfBounds() && !facingInBounds()) {
        /* Deals with boundary condition */
        edgeCollision = true;
        boundCollision();
    } else {
        edgeCollision = false;
    }

    /* Ship to ship collisions */
    monitorShiptoShipCollisions();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorShiptoShipCollisions
 * @purpose: Monitors if ships are about to collide and when they do collide. 
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: None
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorShiptoShipCollisions() {
    float distToShip;
    bool collision;
    /* Loops through the allShips vector to compare their distances to each other */
    for (int i = 0; i < allShips.size(); i++) {

        /* Skips comparision of this object to itself */  
        if (this == allShips.at(i)) {
            continue;
        }

        /* Gets distance between this ship and other enemy ship */
        distToShip = sqrt(pow(allShips.at(i)->getX() - getX(), 2) + pow(allShips.at(i)->getY() - getY(), 2));

        /* If distance is below threshhold, check if ships are colliding */
        if (distToShip <= shipHeight + allShips.at(i)->shipHeight) {
            isShipToShipColliding(allShips.at(i));
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: outOfBounds
 * @purpose: Determines if a ship has hit the boundary of the game or not
 *
 * @parameters: none
 *     
 * @returns: A boolean representing if a ship is colliding with the bounds of the game or not
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool ship::outOfBounds() {
    /* Left and right boundaries */
    if ((getX() <= leftBounds) || (getX() >= rightBounds)) {
        return true;
    }

    /* Up and down boundaries */
    if ((getY() <= upBounds) || (getY() >= downBounds)) {
        return true;
    }

    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: facingInBounds
 * @purpose: Determines if a ship is facing in bounds if it is currently out of bounds
 *
 * @parameters: none
 *     
 * @returns: A boolean representing if a ship is facing in bounds or not
 * @effects: 
 * @notes: Returns true if ship is already in bounds
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool ship::facingInBounds() {

    /* Already in bounds */
    if (!outOfBounds()) {
        return true;
    }

    /* Left boundary */
    if (getX() <= leftBounds) {
        if((rotation > 3 * M_PI_2) || (rotation < M_PI_2)) {
            return true;
        }
    }

    /* right boundary */
    if (getX() >= rightBounds) {
        if((rotation > M_PI_2) && (rotation < 3 * M_PI_2)) {
            return true;
        }
    }

    /* up boundary */
    if (getY() <= upBounds) {
        if((rotation > 0) && (rotation < M_PI)) {
            return true;
        }
    }

    /* down boundary */
    if (getY() >= downBounds) {
        if((rotation > M_PI) && (rotation < 2 * M_PI)) {
            return true;
        }
    }

    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: boundCollision
 * @purpose: After boundary collision, one component of velocity is stripped to zero based on what boundary was collided with
 *
 * @parameters: none
 *     
 * @returns: None
 * @effects: Makes one component of velComp zero
 * @notes: If collision happens then slow ship down extra
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::boundCollision() {
    /* Left and right boundaries */
    if ((getX() <= leftBounds) || (getX() >= rightBounds)) {
        velComp = (Vector2) {0, (float) sinf(rotation) * velMag};
        decelerateShip(collisionDrag);
    }

    /* Up and down boundaries */
    if ((getY() <= upBounds) || (getY() >= downBounds)) {
        velComp = (Vector2) {(float) cosf(rotation) * velMag, 0};
        decelerateShip(collisionDrag);
    }

    /* Corner collision between 2 boundaries */
    if(inCorner() && !facingInBounds()) {
        velMag = 0;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: inCorner
 * @purpose: Determines if the ship is in the corner of the map or not
 *
 * @parameters: none
 *     
 * @returns: a boolean represnting if ship is in corner or not
 * @effects: Makes one component of velComp zero
 * @notes: If collision happens then slow ship down extra
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool ship::inCorner() {
    if ((getX() <= leftBounds && getY() <= upBounds) || (getX() >= rightBounds && getY() <= upBounds) || 
       (getX() <= leftBounds && getY() >= downBounds) || (getX() >= rightBounds && getY() >= downBounds)) {
        return true;
    }
    return false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: sideCannonAttack
 * @purpose: Shoots cannons from the front of the ship
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Inflicts damage on the opponent with front-facing cannons
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::sideCannonAttack(){
    int canonballCounter = 0;
    
    for (int i = 0; i < MAX_SHOTS; i++){
        if (!shoot[i].active){
            //note: need to make the cannons shoot out rather than from middle
            shoot[i].position.x = getX() - (5 * -cos(rotation));
            shoot[i].position.y = getY() - (5 * -sin(rotation));
            shoot[i].active = true;
            /* Angle is based on the direction of the ship */
            if(canonballCounter == 0) {
                shoot[i].speed.x = 3*sin(rotation + (3*(2 * M_PI)) * 7);
                shoot[i].speed.y = 3*cos(rotation + (3*(2 * M_PI)) * 7);
            }
            if (canonballCounter == 1) {
                shoot[i].speed.x = 3*sin(rotation + (3 * M_PI) * 7);
                shoot[i].speed.y = 3*cos(rotation + (3 * M_PI) * 7);
            }
            shoot[i].rot = (rotation);
            canonballCounter++;

            if (canonballCounter == 2) {
                break;
            }  
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: shipShoot
 * @purpose: Shoots cannons from the front of the ship
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Inflicts damage on the opponent with front-facing cannons
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::frontCannonAttack(){
    /* Initializes a canonball and populates space in array with values */
    for (int i = 0; i < MAX_SHOTS; i++) {
        if (!shoot[i].active) {
            shoot[i].position.x = getX() - (40 * -cos(rotation));
            shoot[i].position.y = getY() - (40 * -sin(rotation));
            shoot[i].active = true;
            shoot[i].speed.x = 0.5*sin(rotation + ((3 * M_PI)/2)) *7;
            shoot[i].speed.y = 0.5*cos(rotation + ((3 * M_PI)/ 2)) *7;
            shoot[i].rot = (rotation);
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: fireBarrelAttack
 * @purpose: Drops fire barrels in the water with set times of explosion
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Inflicts damage on the opponent with timed fire barrels
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::fireBarrelAttack() {
    for (int i = 0; i < MAX_SHOTS; i++){
        if (!barrel[i].active) {
            barrel[i].active = true;
            barrel[i].explosionTimer = 7.0f;
            barrel[i].frame = 0;
            barrel[i].position.x = getX() + (60 * -cos(rotation));
            barrel[i].position.y = getY() + (60 * -sin(rotation));
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorCanonballs
 * @purpose:  
 *
 * @parameters: 
 *     
 * @returns:
 * @effects: 
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorCanonballs() {
    
    for (int i = 0; i < MAX_SHOTS; i++){
        if (shoot[i].active) {

            /* Cannon ball timer */
            shoot[i].lifeSpawn++;

            /* Moving the cannonball */
            shoot[i].position.x -= shoot[i].speed.x;
            shoot[i].position.y += shoot[i].speed.y;
            
            /* Collision logic: shoot vs walls */
            if  (shoot[i].position.x > screenWidth + shoot[i].radius){
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }

            else if (shoot[i].position.x < 0 - shoot[i].radius){
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }

            if (shoot[i].position.y > screenHeight + shoot[i].radius){
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }

            else if (shoot[i].position.y < 0 - shoot[i].radius){
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
            }

            /* How long the cannonball appears for */
            if (shoot[i].lifeSpawn >= 60){
                shoot[i].position = (Vector2){0, 0};
                shoot[i].speed = (Vector2){0, 0};
                shoot[i].lifeSpawn = 0;
                shoot[i].active = false;
            }

            /* Drawing the cannon ball */
            DrawCircleV(shoot[i].position, shoot[i].radius, (Color){ 80, 80, 80, 255 });
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorFirebarrel
 * @purpose:  
 *
 * @parameters: 
 *     
 * @returns:
 * @effects: 
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorFirebarrel() {
    for (int i = 0; i < MAX_SHOTS; i++) {
        if (barrel[i].active && isAlive) {
            Rectangle fireBarrelMarker = (Rectangle){barrel[i].position.x, barrel[i].position.y, shipWidth/3, shipHeight/3};
            int damage = EXPLOSION_RADIUS - ((hitBox.x - fireBarrelMarker.x) + (hitBox.y - fireBarrelMarker.y)) * 0.5;
            barrel[i].explosionTimer -= GetFrameTime();

            if (barrel[i].explosionTimer <= 0) {
                if (CheckCollisionCircleRec(barrel[i].position, EXPLOSION_RADIUS, hitBox)) {
                    healthBar -= damage;
                }
                DrawCircle(barrel[i].position.x, barrel[i].position.y, EXPLOSION_RADIUS*2, (Color){ 230, 41, 55, 255 });
                barrel[i].active = false;
            }
            else if (barrel[i].explosionTimer > 0 && barrel[i].active){
                DrawTexture(barrelTexture, barrel[i].position.x - shipWidth/6, barrel[i].position.y - shipHeight/6, (Color){255,255,255,255});
            }
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorShipToWeaponCollisions
 * @purpose:  
 *
 * @parameters: 
 *     
 * @returns:
 * @effects: 
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorShipToWeaponCollisions(){
    for (int i = 0; i < MAX_SHOTS; i++) {
        /* Collision logic between the cannons and enemy ships */
        if ((shoot[i].active) && isAlive) {
            if(CheckCollisionCircleRec(shoot[i].position, shoot[i].radius, hitBox)){
                if ((shoot[i].active)) {
                    if(CheckCollisionCircleRec(shoot[i].position, shoot[i].radius, hitBox)){
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                        healthBar -= 10;
                    }
                }
                /* Explosion logic for the fire barrels and any ships */
                if (barrel[i].active && isAlive) {
                    Rectangle fireBarrelMarker = (Rectangle){barrel[i].position.x, barrel[i].position.y, shipWidth/3, shipHeight/3};
                    int damage = EXPLOSION_RADIUS - ((hitBox.x - fireBarrelMarker.x) + (hitBox.y - fireBarrelMarker.y)) * 0.5;

                    if (CheckCollisionRecs(fireBarrelMarker, hitBox)) { 
                        DrawCircle(barrel[i].position.x, barrel[i].position.y, EXPLOSION_RADIUS*2, (Color){ 230, 41, 55, 255 });
                        barrel[i].active = false;
                        healthBar -= damage;
                    }
                
                    else if (CheckCollisionCircleRec(barrel[i].position, EXPLOSION_RADIUS, hitBox) && isAlive) {
                        DrawCircle(barrel[i].position.x, barrel[i].position.y, EXPLOSION_RADIUS*2, (Color){ 230, 41, 55, 255 });
                        barrel[i].active = false;
                        healthBar -= damage;
                    }
                }
            }
        }
    }
    if (healthBar <= 0) {
        targetRecAlive = false;
        if (spawnLoot) {
            trackLoot = lootDrop();
            loot = (Rectangle){destRec.x, destRec.y, 10, 10};
            spawnLoot = false;
            isAlive = false;
        }
        //one of the ships is disappearing?? ok idk this happened twice
        if (!lootPickedUp) {
            DrawRectangleRec(loot, (Color){ 230, 41, 55, 255 });
            lootExpire -= GetFrameTime();
            if (lootExpire <= 0) {
                spawnTypes[trackLoot]--;
            }
        }
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorCoolDown
 * @purpose: Monitors the cooldown time 
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Reduces the cooldown by the frame time and sets shotFired to false if cooldown reaches 0
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::monitorCoolDown() {

    /* Side cannon cooldown */
    if (sideCannonsUnlocked) {
        sideCannonsCooldown -= GetFrameTime();
        if (sideCannonsCooldown <= 0) {
            sideCannonsCooldown = 0;
            sideCannonsAvailable = true;
        }
    }

    /* Front cannon cooldown */
    if (frontCannonUnlocked) {
        frontCannonCooldown -= GetFrameTime();
        if (frontCannonCooldown <= 0) {
            frontCannonCooldown = 0;
            frontCannonAvailable = true;
        }
    }
    
    /* Fire barrel cooldown */
    if (fireBarrelUnlocked) {
        fireBarrelCooldown -= GetFrameTime();
        if (fireBarrelCooldown <= 0) {
            fireBarrelCooldown = 0;
            fireBarrelAvailable = true;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: lootDrop
 * @purpose: Monitors the cooldown time 
 * @effects: Reduces the cooldown by the frame time and sets shotFired to false if cooldown reaches 0
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int ship::lootDrop() {
    std::minstd_rand generator(std::time(0)); 
    std::uniform_int_distribution<> type(0, 4);

    int nextRandomInt = type(generator);

    if (nextRandomInt == 0) {
        spawnTypes[0]++;
        return 0;
    }
    else if (nextRandomInt == 1) {
        spawnTypes[1]++;
        return 1;
    }
    else if (nextRandomInt == 2) {
        spawnTypes[2]++;
        return 2;
    }
    else if (nextRandomInt == 3) {
        spawnTypes[3]++;
        return 3;
    }
    else {
        spawnTypes[4]++;
        return 4;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: isShipToShipColliding
 * @purpose: Determines if two ships are colliding
 *
 * @parameters: ship* otherShip: a pointer to another ship to check collision with
 *     
 * @returns: nothing
 * @effects: None
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::isShipToShipColliding(ship* otherShip) {
    /* Gets the minimum separation between the two ships */
    shipCollisionStruct thisCollision = findMinSeparation(this, otherShip);
    shipCollisionStruct otherCollision = findMinSeparation(otherShip, this);

    /* If separation amount is < 0, collision is happening */
    if ((thisCollision.separation <= 0) && (otherCollision.separation <= 0)) {
        shipToShipCollision(otherCollision.collidingEdge);
        collision = otherCollision;
    } else {
        shipCollision = false;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: findMinSeparation
 * @purpose: Finds the minimum separation between 2 ships using Separating Axis Theorem
 *
 * @parameters: ship* shipA: a pointer to a ship to check collision with
 *              ship* shipB: a pointer to a ship to check collision with
 *    
 * @returns: a shipCollision struct with the minimum separation amount and the corresponding edge from shipA
 *           this separation was found from
 * @effects: None
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
shipCollisionStruct ship::findMinSeparation(ship* shipA, ship* shipB) {
    float separation = std::numeric_limits<float>::lowest();
    Vector2 normal;
    float minSep;
    shipCollisionStruct thisCollision;
    thisCollision.separation = separation;
    thisCollision.collidingEdge = (Vector2){0, 0};

    /* Loop through all edges in shipA */
    /* Note: since hitBoxes are rectangles, only need to go through 2 of the edges, not all 4 */
    for (int i = 0; i < shipA->hitBoxEdges.size(); i++) {
        normal = (Vector2){shipA->hitBoxEdges.at(i).y, -shipA->hitBoxEdges.at(i).x};
        minSep = std::numeric_limits<float>::max();

        /* Loop through all vertices in shipA and shipB */
        for (int j = 0; j < shipA->hitBoxVertices.size(); j++) {
            for (int k = 0; k < shipB->hitBoxVertices.size(); k++) {
                minSep = std::min(minSep, Vector2DotProduct(Vector2Subtract(shipA->hitBoxVertices.at(j), shipB->hitBoxVertices.at(k)), normal));
            }
        }

        if (separation < minSep) {
            separation = minSep;
            thisCollision.separation = separation;
            thisCollision.collidingEdge = shipA->hitBoxEdges.at(i);
        }
    } 
    return thisCollision;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: shipToShipCollision
 * @purpose: Updates the velocity components of this ship during a collision
 *
 * @parameters: Vector2 collidingEdge: a Vector2 representing the edge this ship is colliding with
 *    
 * @returns: none
 * @effects: Updates velMag and velComp of this ship
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::shipToShipCollision(Vector2 collidingEdge) {
    /* Unit vector of where this ship is pointing */
    Vector2 inDirection = Vector2One();
    inDirection.x = cos(rotation);
    inDirection.y = sin(rotation);
    
    /* Unit vector of colliding edge */
    Vector2 collidingEdgeNormalized = Vector2Normalize(collidingEdge);

    /* If hasn't collided yet, update velMag */
    if (!shipCollision) {
        velMag = velMag * abs(Vector2DotProduct(inDirection, collidingEdgeNormalized));
        shipCollision = true;
    }
    decelerateShip(drag);

    /* Determines angle between edge vector and vector of where this ship is pointing to */
    float dot = Vector2DotProduct(inDirection, collidingEdgeNormalized);
    float det = (inDirection.x * collidingEdgeNormalized.y) - (inDirection.y * collidingEdgeNormalized.x);
    float angle = atan2(dot, det);
    angle = fmod(angle + (2 * M_PI), 2 * M_PI);

    /* Updates the velocity of this ship based on angle */
    if (Vector2DotProduct(inDirection, collidingEdgeNormalized) > 0) {
        if (angle < M_PI_2 || angle > 3 * M_PI_2) {
            velComp.x = collidingEdgeNormalized.x * velMag;
            velComp.y = collidingEdgeNormalized.y * velMag;
        }
    } else {
        if (angle < M_PI_2 || angle > 3 * M_PI_2) {
            velComp.x = -collidingEdgeNormalized.x * velMag;
            velComp.y = -collidingEdgeNormalized.y * velMag;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: computeHitBox
 * @purpose: Computes the hitbox of the ship
 *
 * @parameters: none
 *    
 * @returns: none
 * @effects: Sets the values of the hitBoxEdges and hitBoxVertices vector
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::computeHitBox() {
    /* center point of the hitbox */
    int hitBoxCenterX = destRec.x;
    int hitBoxCenterY = destRec.y;

    /* Vertices of the hitbox */
    /* Top left vertex */
    Vector2 vertexTL = (Vector2){hitBoxCenterX + (hitBox.x + (shipWidth * 0.1) - hitBoxCenterX) * cos(-rotation) + (hitBox.y + (shipHeight * 0.2) - hitBoxCenterY) * sin(-rotation), 
                                hitBoxCenterY - (hitBox.x + (shipWidth * 0.1) - hitBoxCenterX) * sin(-rotation) + (hitBox.y + (shipHeight * 0.2) - hitBoxCenterY) * cos(-rotation)};
    /* Top right vertext */
    Vector2 vertexTR = (Vector2){hitBoxCenterX + (hitBox.x + (shipWidth * 0.9) - hitBoxCenterX) * cos(-rotation) + (hitBox.y + (shipHeight * 0.2) - hitBoxCenterY) * sin(-rotation), 
                                hitBoxCenterY - (hitBox.x + (shipWidth * 0.9) - hitBoxCenterX) * sin(-rotation) + (hitBox.y + (shipHeight * 0.2) - hitBoxCenterY) * cos(-rotation)};
    /* Bottom left vertex */
    Vector2 vertexBL = (Vector2){hitBoxCenterX + (hitBox.x + (shipWidth * 0.1) - hitBoxCenterX) * cos(-rotation) + (hitBox.y + (shipHeight * 0.8) - hitBoxCenterY) * sin(-rotation), 
                                hitBoxCenterY - (hitBox.x + (shipWidth * 0.1) - hitBoxCenterX) * sin(-rotation) + (hitBox.y + (shipHeight * 0.8) - hitBoxCenterY) * cos(-rotation)};
    /* Bottom right vertext */
    Vector2 vertexBR = (Vector2){hitBoxCenterX + (hitBox.x + (shipWidth * 0.9) - hitBoxCenterX) * cos(-rotation) + (hitBox.y + (shipHeight * 0.8) - hitBoxCenterY) * sin(-rotation), 
                                hitBoxCenterY - (hitBox.x + (shipWidth * 0.9) - hitBoxCenterX) * sin(-rotation) + (hitBox.y + (shipHeight * 0.8 ) - hitBoxCenterY) * cos(-rotation)};

    /* Adds vertices to hitBoxVertices vector */
    hitBoxVertices.at(0) = vertexTL;
    hitBoxVertices.at(1) = vertexTR;
    hitBoxVertices.at(2) = vertexBR;
    hitBoxVertices.at(3) = vertexBL;

    /* Adds edges to hitBoxEdges vector */
    hitBoxEdges.at(0) = Vector2Subtract(vertexTL, vertexTR);
    hitBoxEdges.at(1) = Vector2Subtract(vertexTR, vertexBR);
    hitBoxEdges.at(2) = Vector2Subtract(vertexBR, vertexBL);
    hitBoxEdges.at(3) = Vector2Subtract(vertexBL, vertexTL);
}