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
player::player(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    playerWidth = this->screenWidth / 22;
    playerHeight = this->screenHeight / 11;
    rotation = 0;
    velMag = 0.0;

    /* Loads in image and resizes it for texture */
    Image sprite = LoadImage("./images/pirateShip.png");
    ImageResize(&sprite, playerWidth, playerHeight);
    ImageRotateCW(&sprite);
    playerTexture = LoadTextureFromImage(sprite); 
    UnloadImage(sprite);
    playerWidth = playerTexture.width;
    playerHeight = playerTexture.height;

    /* Source rectangle (part of the texture to use for drawing) */
    sourceRec = (Rectangle){0.0, 0.0, playerWidth, playerHeight};

    /* Destination rectangle (screen rectangle where drawing part of texture) */
    destRec = (Rectangle){this->screenWidth / 2, this->screenHeight / 2, playerWidth, playerHeight};

    /* Origin of the texture (rotation/scale point) */
    origin = (Vector2){playerWidth / 2, playerHeight / 2};

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
 * @function: drawPlayer
 * @purpose: Draws player on the screen
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Updates the destRec x and y components based on velocity and rotation
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::drawPlayer() {

    
    Vector2 velComp = {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
    destRec.x += velComp.x;
    destRec.y += velComp.y;

    /* Draws player on screen
    Note: rotation is multiplied by FPS to compensate for the BeginDrawing function */
    DrawTexturePro(playerTexture, sourceRec, destRec, origin, rotation * GetFPS(), (Color){255,255,255,255});

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
    /* Monitor for buttons pressed */
    if (IsKeyDown(KEY_UP)) {
        up = true;
    } else {
        up = false;
    }

    if (IsKeyDown(KEY_LEFT)) {
        left = true;
    } else {
        left = false;
    }

    if (IsKeyDown(KEY_DOWN) && velMag > 0) {
        down = true;
    } else {
        down = false;
    }

    if (IsKeyDown(KEY_RIGHT)) {
        right = true;
    } else {
        right = false;
    }

    /* move player and deal with kinematics */
    movePlayer();
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: movePlayer
 * @purpose: Moves the player on the screen based on arrow key buttons being pressed
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Player position and orientation changes
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::movePlayer(){
    /* Note for later: if rotation hits a limit, reset */
    if (right) {
        rotation += 0.01;
        if (velMag > 0) {
            velMag -= 0.01;
        }
    }

    if (left) {
        rotation -= 0.01;
        if (velMag > 0) {
            velMag -= 0.01;
        }
    }

    if (up) {
        velMag += 0.015;
    } else if (velMag > 0) {
        /* drag force */ velMag -= 0.01;
    
    }

    if (down) {
        velMag -= 0.0025;
    }

    if (velMag < 0.0) {
        velMag = 0.0;
    }
}