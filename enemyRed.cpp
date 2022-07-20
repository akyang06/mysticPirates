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
enemyRed::enemyRed(int screenWidth, int screenHeight, const char* texturePath) : enemy(screenWidth, screenHeight, texturePath) {
    range = 150;
    velLimit = 2.5;

    acceration = 0.015;
    deceleration = 0.0125;
    turnDrag = 0.005;
    drag = 0.0025;
    rotationSpeed = 0.01;

    int caseX = rand() % 2;
    int caseY = rand() % 2;

    if (caseX == 0) {
        destRec.x = (rand() % 300) + screenWidth + 100;
    } else {
        destRec.x = -(rand() % 300) - 100;
    }

    if (caseY == 0) {
        destRec.y = (rand() % 300) + screenHeight + 100;
    } else {
        destRec.y = -(rand() % 300) - 100;
    }
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

