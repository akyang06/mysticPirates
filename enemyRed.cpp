/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  enemyRed.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function implementations of enemy class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "enemyRed.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: constructor
 * @purpose: Initializes an enemy object and loads in the image of the enemy's ship with correct dimensions
 *
 * @parameters: the screen width and height
 *     
 * @returns: nothing
 * @effects: sets the values of playerWidth and playerHeight, also defines the playerTexture
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
enemyRed::enemyRed() : enemy() {

    /* Enemy Red ship initial stats */
    acceration = 0.015;
    deceleration = 0.01;
    turnDrag = 0.005;
    drag = 0.0025;
    velLimit = 2.5;
    rotationSpeed = 0.01;
    range = 150;

    /* Loads in image and resizes it for texture */
    Image sprite = LoadImage("images/enemyRed.png");
    shipWidth = screenWidth / 22;
    shipHeight = screenHeight / 11;
    ImageResize(&sprite, shipWidth, shipHeight);
    ImageRotateCW(&sprite);
    shipTexture = LoadTextureFromImage(sprite); 
    UnloadImage(sprite);
    shipWidth = shipTexture.width;
    shipHeight = shipTexture.height;

    /* Source rectangle (part of the texture to use for drawing) */
    sourceRec = (Rectangle){0.0, 0.0, shipWidth, shipHeight};

    /* Destination rectangle (screen rectangle where drawing part of texture) */
    destRec.width = shipWidth;
    destRec.height = shipHeight;

    /* Origin of the texture (rotation/scale point) */
    origin = (Vector2){shipWidth / 2, shipHeight / 2};

    /* Determines the boundary of the screen that the hsip should stay in */
    upBounds = shipHeight / 4;
    downBounds = screenHeight - (shipHeight / 2);
    leftBounds = shipWidth / 4;
    rightBounds = screenWidth - (shipWidth / 4);    
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
enemyRed::~enemyRed() {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorEnemy
 * @purpose: Monitor the distance between the enemy ship and the players ship to move the enemy towards the player
 *
 * @parameters: p1: the player's ship
 *     
 * @returns: nothing
 * @effects: none
 * @notes:   Calls rotateEnemy() and moveEnemy() function to move the enemy
 *           Moves enemy into the screen after they spawn
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::monitorEnemyRed(player p1) {

    distToPlayer = (Vector2) {p1.getX() - getX(), p1.getY() - getY()};
    distMag = sqrt(pow(distToPlayer.x, 2) + pow(distToPlayer.y, 2));
    

    /* Moves enemy into the bounds after they spawn and continue to move after */
    if (!enteredBounds) {
        moveEnemyInBounds();
    } else {
        moveEnemyRed();
    } 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: moveEnemy
 * @purpose: Moves the enemy based on its distance to the player ship
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy position and orientation changes
 * @notes:   The enemy moves closer to the ship until it gets into a certain radius of it
 *           Calls the accelerateShip and decelerateShip
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::moveEnemyRed(){
    DrawText(TextFormat("distMag: %f", distMag), 20, 50, 20, (Color){255,255,255,255});

    if (outOfBounds() && !facingInBounds()) {
        boundCollision();
    }

    if (distMag > 1.5 * range) {
        facePlayer();
        accelerateShip(acceration);
    }
    // } else if (distMag > range) {
    //     alignWithPlayer();
    // } else {
    //     attackPlayer();
    // }
}

