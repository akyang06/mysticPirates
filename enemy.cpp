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
enemy::enemy(int screenWidth, int screenHeight) : ship(screenWidth, screenHeight) {
    range = 100;
    velLimit = 1.5;

    acceration = 0.015;
    deceleration = 0.0125;
    turnDrag = 0.005;
    drag = 0.0025;

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
 * @function: monitorEnemy
 * @purpose: Monitor the distance between the enemy ship and the players ship to move the enemy towards the player
 *
 * @parameters: p1: the player's ship
 *     
 * @returns: nothing
 * @effects: none
 * @notes:   Calls rotateEnemy() and moveEnemy() function to move the enemy
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::monitorEnemy(player p1) {

    distToPlayer = (Vector2) {p1.getX() - getX(), p1.getY() - getY()};
    distMag = sqrt(pow(distToPlayer.x, 2) + pow(distToPlayer.y, 2));
    /* move enemy and deal with kinematics */
    rotateEnemy();
    moveEnemy();
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
 *           Calls the accelerateEnemy and slowDownEnemy
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::moveEnemy(){
    DrawText(TextFormat("distMag: %f", distMag), 20, 50, 20, (Color){255,255,255,255});

    if (outOfBounds()) {
        velMag = 0;
    }

    if (distMag > 1.5 * range) {
        accelerateShip(acceration);
    } 
    else if (distMag > 50 && distMag < 250) {
        decelerateShip(1.025);

    } 
    else {
        decelerateShip(deceleration);
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: rotateEnemy
 * @purpose: Rotates the enemy ship so it is facing the player's ship
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy orientation changes
 * @notes:   rotation angle is between 0 and 2pi radians
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::rotateEnemy() {

    /* Gets required angle to face player */
    float angleReq = atan2(distToPlayer.y, distToPlayer.x);
    angleReq = fmod(angleReq + (2 * M_PI), 2 * M_PI);
    
    /* Moves ship counter clockwise from 0 over to 2pi radians */
    if ((2 * M_PI - angleReq) + rotation < angleReq - rotation) {
        rotation -= 0.01;
        decelerateShip(turnDrag);      
    } /* Moves ship clockwise from 2pi over to 0 radians */
    else if ((2 * M_PI - rotation) + angleReq < rotation - angleReq) {
        rotation += 0.01;
        decelerateShip(turnDrag);
    } /* Moves ship clockwise normally */
    else if (angleReq - rotation > 0.01) {
        rotation += 0.01; 
        decelerateShip(turnDrag);
    } /* Moves ship counterclockwise normally */
    else if (angleReq - rotation < -0.01) {
        rotation -= 0.01;  
        decelerateShip(turnDrag); 
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}