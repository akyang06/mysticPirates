/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  enemy.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  07/16/22
 *
 *  
 *
 *  Function implementations of enemy class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "enemy.h"

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
enemy::enemy() : ship() {

    /* Determines random position for enemy ship to spawn in */

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
enemy::~enemy() {

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: moveEnemyInBounds
 * @purpose: Move the enemy in bounds when they spawn into the game
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: 
 * @notes: after spawn, moves enemy into the bounds
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::moveEnemyInBounds() {
    facePlayer();
    accelerateShip(acceration);
    if (!outOfBounds()) {
        enteredBounds = true;
    }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: facePlayer
 * @purpose: Rotates the enemy ship so it is facing the player's ship
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy orientation changes
 * @notes:   rotation angle is between 0 and 2pi radians
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::facePlayer() {

    /* Gets required angle to face player */
    float angleReq = atan2(distToPlayer.y, distToPlayer.x);
    angleReq = fmod(angleReq + (2 * M_PI), 2 * M_PI);
    
    /* Moves ship counter clockwise from 0 over to 2pi radians */
    if ((2 * M_PI - angleReq) + rotation < angleReq - rotation) {
        rotation -= rotationSpeed;
        decelerateShip(turnDrag);      
    } /* Moves ship clockwise from 2pi over to 0 radians */
    else if ((2 * M_PI - rotation) + angleReq < rotation - angleReq) {
        rotation += rotationSpeed;
        decelerateShip(turnDrag);
    } /* Moves ship clockwise normally */
    else if (angleReq - rotation > rotationSpeed) {
        rotation += rotationSpeed; 
        decelerateShip(turnDrag);
    } /* Moves ship counterclockwise normally */
    else if (angleReq - rotation < rotationSpeed) {
        rotation -= rotationSpeed;  
        decelerateShip(turnDrag); 
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}