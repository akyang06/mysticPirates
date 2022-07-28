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

#include "ship.h"

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

    Cannonballs allCannonballs[MAX_SHOTS] = { 0 };

    /* Initializes the shooting component */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
       allCannonballs[i].position = (Vector2){0, 0};
       allCannonballs[i].speed = (Vector2){0, 0};
       allCannonballs[i].radius = 5;
       allCannonballs[i].active = false;
       allCannonballs[i].lifeSpawn = 0;
       allCannonballs[i].color = (Color){ 80, 80, 80, 255 };
    }

    /* Initializes the attack type to cannon */
    shootType = 1;
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

    targetRec.x = destRec.x - destRec.width/2;
    targetRec.y = destRec.y - destRec.height/2;

    /* Draws ship on screen
    Note: rotation is multiplied by FPS to compensate for the BeginDrawing function */
    DrawTexturePro(shipTexture, sourceRec, destRec, origin, rotation * 57.3, (Color){255,255,255,255});
    DrawCircleLines(destRec.x, destRec.y, 250, (Color){0,0,0,255});

    DrawRectangleRec(targetRec, (Color){ 0, 82, 172, 255 });

    /* Health bar status */
    DrawRectangle((targetRec.x), (targetRec.y - 50), healthBar, 7, (Color){ 0, 228, 48, 255 });
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
 * @function: getCannonballs 
 * @purpose: Returns a pointer to the array of cannonballs of the ship
 *
 * @parameters: none
 *     
 * @returns: a pointer to the array of cannonballs of the ship
 * @effects: 
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Cannonballs* ship::getCannonballs() {
    Cannonballs *cannonballsPtr = allCannonballs;
    return cannonballsPtr;
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
 * @function: frontShipShoot
 * @purpose: Shoots cannons from the front of the ship
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Inflicts damage on the opponent with front-facing cannons
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::frontShipShoot(){
    if ((IsKeyPressed(KEY_SPACE))){
        for (int i = 0; i < MAX_SHOTS; i++)
        {
            if (!allCannonballs[i].active)
            {
               allCannonballs[i].position = (Vector2){ getX(), getY()};
               allCannonballs[i].active = true;
                /* Angle is based on the direction of the ship */
                if (shootType == 1) {
                   allCannonballs[i].speed.x = 0.5*sin(rotation + ((3 * M_PI)/2)) *7;
                   allCannonballs[i].speed.y = 0.5*cos(rotation + ((3 * M_PI)/ 2)) *7;
                }
                else if (shootType == 2) {
                   allCannonballs[i].speed.x = 3*sin(rotation + (3 * M_PI) * 7);
                   allCannonballs[i].speed.y = 3*cos(rotation + (3 * M_PI) * 7);
                }
                
               allCannonballs[i].rot = (rotation);
                break;
            }
        }
    }

    /* Cannon ball timer */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active)allCannonballs[i].lifeSpawn++;
    }

    /* Logic for shooting */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active)
        {
            /* Moving the "cannon ball" */
           allCannonballs[i].position.x -=allCannonballs[i].speed.x;
           allCannonballs[i].position.y +=allCannonballs[i].speed.y;
             // Collision logic: shoot vs walls
            if  (allCannonballs[i].position.x > screenWidth +allCannonballs[i].radius){
               allCannonballs[i].active = false;
               allCannonballs[i].lifeSpawn = 0;
            }

            else if (allCannonballs[i].position.x < 0 -allCannonballs[i].radius){
               allCannonballs[i].active = false;
               allCannonballs[i].lifeSpawn = 0;
            }

            if (allCannonballs[i].position.y > screenHeight +allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            else if (allCannonballs[i].position.y < 0 - allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            /* How long the "cannon ball" appears for */
            if (allCannonballs[i].lifeSpawn >= 60)
            {
               allCannonballs[i].position = (Vector2){0, 0};
               allCannonballs[i].speed = (Vector2){0, 0};
               allCannonballs[i].lifeSpawn = 0;
               allCannonballs[i].active = false;
            }
        }
    }
    /* Drawing the cannon ball */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active) DrawCircleV(allCannonballs[i].position, allCannonballs[i].radius, (Color){ 80, 80, 80, 255 });
    }
    sideShipShoot();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: sideShipShoot
 * @purpose: Shoots cannons from the front of the ship
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Inflicts damage on the opponent with front-facing cannons
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ship::sideShipShoot(){
    if ((IsKeyPressed(KEY_SPACE)) && shootType == 2){
        for (int i = 0; i < MAX_SHOTS; i++)
        {
            if (!allCannonballs[i].active)
            {
              
                allCannonballs[i].position = (Vector2){ getX(), getY()};
                allCannonballs[i].active = true;
                /* Angle is based on the direction of the ship */
                allCannonballs[i].speed.x = 3*sin(rotation + (3*(2 * M_PI)) * 7);
                allCannonballs[i].speed.y = 3*cos(rotation + (3*(2*M_PI)) * 7);
                allCannonballs[i].rot = (rotation);
                break;
            }
        }
    }

    /* Cannon ball timer */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active)allCannonballs[i].lifeSpawn++;
    }

    /* Logic for shooting */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active)
        {
            /* Moving the "cannon ball" */
            allCannonballs[i].position.x -= allCannonballs[i].speed.x;
            allCannonballs[i].position.y += allCannonballs[i].speed.y;
             // Collision logic: shoot vs walls
            if  (allCannonballs[i].position.x > screenWidth + allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            else if (allCannonballs[i].position.x < 0 - allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            if (allCannonballs[i].position.y > screenHeight + allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            else if (allCannonballs[i].position.y < 0 - allCannonballs[i].radius){
                allCannonballs[i].active = false;
                allCannonballs[i].lifeSpawn = 0;
            }

            /* How long the "cannon ball" appears for */
            if (allCannonballs[i].lifeSpawn >= 60)
            {
                allCannonballs[i].position = (Vector2){0, 0};
                allCannonballs[i].speed = (Vector2){0, 0};
                allCannonballs[i].lifeSpawn = 0;
                allCannonballs[i].active = false;
            }
        }
    }
    /* Drawing the cannon ball */
    for (int i = 0; i < MAX_SHOTS; i++)
    {
        if (allCannonballs[i].active) DrawCircleV(allCannonballs[i].position, allCannonballs[i].radius, (Color){ 80, 80, 80, 255 });
    }
}
