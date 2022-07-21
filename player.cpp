/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  player.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  06/28/22
 *
 *  
 *
 *  Function implementations of player class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "player.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: constructor
 * @purpose: Initializes a player object and loads in the base image of the player's ship with correct dimensins
 *
 * @parameters: the screen width and height
 *     
 * @returns: nothing
 * @effects: sets the values of playerWidth and playerHeight, also defines the playerTexture
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
player::player() : ship() {

    /* Player ship initial stats */
    acceration = 0.015;
    deceleration = 0.01;
    turnDrag = 0.005;
    drag = 0.0025;
    velLimit = 5;
    rotationSpeed = 0.01;

    /* Loads in image and resizes it for texture */
    Image sprite = LoadImage("images/starterShip.png");
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
    destRec = (Rectangle){-150, screenHeight / 2, shipWidth, shipHeight};

    /* Origin of the texture (rotation/scale point) */
    origin = (Vector2){shipWidth / 2, shipHeight / 2};

    /* Determines the boundary of the screen that the ship should stay in */
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
player::~player() {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: enterPlayer
 * @purpose: Player enters the game screen before the level starts
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: none
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::enterPlayer() {

    destRec.x += 2;
    if (destRec.x >= screenWidth / 2) {
        enteredBounds = true;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorPlayer
 * @purpose: Monitor the user for any keys being pressed to then move the ship
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: none
 * @notes:   Calls movePlayer() function to move the player
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::monitorPlayer() {

    /* move player and deal with kinematics */
    if (!enteredBounds) {
        enterPlayer();
    } else {
        rotatePlayer();
        movePlayer();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: movePlayer
 * @purpose: Moves the player on the screen based on arrow key buttons being pressed
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Player position changes
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::movePlayer(){

    if (IsKeyDown(KEY_UP)) {
        accelerateShip(acceration);
    } else if (velMag > 0) {
        /* drag force */
        decelerateShip(drag);
    }

    if (IsKeyDown(KEY_DOWN)) {
        decelerateShip(deceleration);
    }

    if (outOfBounds() && !facingInBounds()) {
        boundCollision();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: rotatePlayer
 * @purpose: Rotate the player on the screen based on arrow key buttons being pressed
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Player orientation changes
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::rotatePlayer(){
    if (IsKeyDown(KEY_RIGHT)) {
        rotation += rotationSpeed;
        decelerateShip(turnDrag);
    }

    if (IsKeyDown(KEY_LEFT)) {
        rotation -= rotationSpeed;
        decelerateShip(turnDrag);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}