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

    /* Initializes the shooting component */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        shoot[i].position = (Vector2){0, 0};
        shoot[i].speed = (Vector2){0, 0};
        shoot[i].radius = 5;
        shoot[i].active = false;
        shoot[i].lifeSpawn = 0;
        //shoot[i].color = (Color){ 80, 80, 80, 255 };
    }

    /* Initializes the attack type to cannon */
    isAlive = true;

    /* Target rectangle initialized as true */
    targetRecAlive = true;
    lootSpawn = false;
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
    //UnloadTexture(barrelTexture);
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
        targetRec.x = destRec.x - destRec.width/2;
        targetRec.y = destRec.y - destRec.height/2;
    }

    /* Draws ship on screen
    Note: rotation is multiplied by FPS to compensate for the BeginDrawing function */

    if (healthBar > 0 && targetRecAlive) {
        DrawTexturePro(shipTexture, sourceRec, destRec, origin, rotation * 57.3, (Color){255,255,255,255});
        DrawCircleLines(destRec.x, destRec.y, 250, (Color){0,0,0,255});
        isAlive = false;

        /* Health bar status */
        DrawRectangle(targetRec.x - 7, targetRec.y - 17, healthBar, 7, (Color){ 0, 228, 48, 255 });
    }
    // else {
    //     // need to make a condition here where the loot disappears after ship hovered over
    //     // std::minstd_rand generator(std::time(0)); 
    //     // std::uniform_int_distribution<> numLoot(0, 4);

    //     // int nextRandomInt = numLoot(generator);

    //     if (lootSpawn == false) {
    //         lootTypeColor = lootDrop();
    //         targetRecAlive = false;
    //         lootSpawn = true;
    //     }


    //     // if (drawLoot) {
    //     //     dropPoint = (Rectangle){targetRec.x, targetRec.y, 10, 10};
    //     //     DrawRectangleRec(dropPoint, (Color){ 255, 255, 255, 255 });
    //     // }
    //     // DrawText(TextFormat("red count: %i", red), 100, 100, 25, (Color){255,255,255,255});
    // }
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
 * @function: getRotatio 
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

    /* Deals with velocity updates when colliding with bounds */
    if (outOfBounds() && !facingInBounds()) {
        /* Left and right boundaries */
        if ((getX() <= leftBounds) || (getX() >= rightBounds)) {
            velComp = (Vector2) {0, (float) sinf(rotation) * velMag};
        }

        /* Up and down boundaries */
        if ((getY() <= upBounds) || (getY() >= downBounds)) {
            velComp = (Vector2) {(float) cosf(rotation) * velMag, 0};
        }
        return;
    }

    velComp = (Vector2) {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
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

    /* Deals with velocity updates when colliding with bounds */
    if (outOfBounds() && !facingInBounds()) {
        /* Left and right boundaries */
        if ((getX() <= leftBounds) || (getX() >= rightBounds)) {
            velComp = (Vector2) {0, (float) sinf(rotation) * velMag};
        }

        /* Up and down boundaries */
        if ((getY() <= upBounds) || (getY() >=  downBounds)) {
            velComp = (Vector2) {(float) cosf(rotation) * velMag, 0};
        }
        return;
    }

    velComp = (Vector2) {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
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
 * @notes: Returns false if ship is already in bounds
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool ship::facingInBounds() {
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
        velComp.x = 0;
        decelerateShip(collisionDrag);
    }

    /* Up and down boundaries */
    if ((getY() <= upBounds) || (getY() >= downBounds)) {
        velComp.y = 0;
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
       (getX() <= leftBounds && getY() >= downBounds) || (getX() >= rightBounds && (getY()) >= downBounds)) {
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
        if (barrel[i].active && targetRecAlive) {
            Rectangle fireBarrelMarker = (Rectangle){barrel[i].position.x, barrel[i].position.y, shipWidth/3, shipHeight/3};
            int damage = EXPLOSION_RADIUS - ((targetRec.x - fireBarrelMarker.x) + (targetRec.y - fireBarrelMarker.y)) * 0.5;
            barrel[i].explosionTimer -= GetFrameTime();

            if (barrel[i].explosionTimer <= 0) {
                if (CheckCollisionCircleRec(barrel[i].position, EXPLOSION_RADIUS, targetRec)) {
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
 * @function: checkCollision
 * @purpose:  
 *
 * @parameters: 
 *     
 * @returns:
 * @effects: 
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::checkCollision(){
    for (int i = 0; i < MAX_SHOTS; i++) {
        /* Collision logic between the cannons and enemy ships */
        if ((shoot[i].active) && targetRecAlive) {
            if(CheckCollisionCircleRec(shoot[i].position, shoot[i].radius, targetRec)){
                shoot[i].active = false;
                shoot[i].lifeSpawn = 0;
                healthBar -= 10;
            }
        }
        /* Explosion logic for the fire barrels and any ships */
        if (barrel[i].active && targetRecAlive) {
            Rectangle fireBarrelMarker = (Rectangle){barrel[i].position.x, barrel[i].position.y, shipWidth/3, shipHeight/3};
            int damage = EXPLOSION_RADIUS - ((targetRec.x - fireBarrelMarker.x) + (targetRec.y - fireBarrelMarker.y)) * 0.5;

            if (CheckCollisionRecs(fireBarrelMarker, targetRec)) { 
                DrawCircle(barrel[i].position.x, barrel[i].position.y, EXPLOSION_RADIUS*2, (Color){ 230, 41, 55, 255 });
                barrel[i].active = false;
                healthBar -= damage;
            }
        
            else if (CheckCollisionCircleRec(barrel[i].position, EXPLOSION_RADIUS, targetRec) && targetRecAlive) {
                DrawCircle(barrel[i].position.x, barrel[i].position.y, EXPLOSION_RADIUS*2, (Color){ 230, 41, 55, 255 });
                barrel[i].active = false;
                healthBar -= damage;
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

// void ship::unloadComponents() {
//     UnloadTexture(sprite);
//     UnloadTexture(barrelTexture);
// }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: lootDrop
 * @purpose: Monitors the cooldown time 
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Reduces the cooldown by the frame time and sets shotFired to false if cooldown reaches 0
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Color ship::lootDrop() {
    drawLoot = true;
    std::minstd_rand generator(std::time(0)); 
    std::uniform_int_distribution<> type(0, 4);

    int nextRandomInt = type(generator);

    if (nextRandomInt == 0) {
        lootTypeColor = (Color){ 230, 41, 55, 255 };
        lootTypeInt = 1;
    }
    else if (nextRandomInt == 1) {
        lootTypeColor = (Color){ 255, 161, 0, 255 };
        lootTypeInt = 2;
    }
    else if (nextRandomInt == 2) {
        lootTypeColor = (Color){ 253, 249, 0, 255 };
        lootTypeInt = 3;
    }
    else if (nextRandomInt == 3) {
        lootTypeColor = (Color){ 102, 191, 255, 255};
        lootTypeInt = 4;
    }
    else {
        lootTypeColor = (Color){ 200, 122, 255, 255 };
        lootTypeInt = 5;
    }

    return lootTypeColor;
}

void ship::lootPickup() {
    if(CheckCollisionRecs(targetRec, dropPoint)) {
        DrawText(TextFormat("collision"), 100, 100, 25, (Color){255,255,255,255});
    }
}