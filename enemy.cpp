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
enemy::enemy(int screenWidth, int screenHeight, const char* texturePath) : ship(screenWidth, screenHeight, texturePath) {
    
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
    accelerateShip(acceration);
    if (!outOfBounds()) {
        enteredBounds = true;
    }
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
void enemy::monitorEnemy(player p1) {

    distToPlayer = (Vector2) {p1.getX() - getX(), p1.getY() - getY()};
    distMag = sqrt(pow(distToPlayer.x, 2) + pow(distToPlayer.y, 2));

    /* Rotates enemy to face player */
    rotateEnemy();

    /* Moves enemy into the bounds after they spawn and continue to move after */
    if (!enteredBounds) {
        moveEnemyInBounds();
    } else {
        moveEnemy();
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
void enemy::moveEnemy(){
    DrawText(TextFormat("distMag: %f", distMag), 20, 50, 20, (Color){255,255,255,255});

    if (outOfBounds() && !facingInBounds()) {
        boundCollision();
    }

    if (distMag > 1.5 * range) {
        accelerateShip(acceration);
    } 
    else if (distMag > 50 && distMag < 250) {
        decelerateShip(1.025);
        /* Gets required angle to face player */
        float angleReq = atan2(distToPlayer.y, distToPlayer.x);
        angleReq = fmod(angleReq + (2 * M_PI), 2 * M_PI);
        
        /* Moves ship counter clockwise from 0 over to 2pi radians */
        if ((2 * M_PI - angleReq) < angleReq) {
        //if (rotation > 0.0 && rotation < 1.75) {
            rotation -= 0.05;
       // }
        // else if (rotation > 1.75 && rotation < 3.5) {
        //     rotation -= 0.045;
        // }
        // else if (rotation > 3.5 && rotation < 4.75) {
        //     rotation += 0.045;
        // }
        // else{
        //     rotation += 0.045;
        // }
        }
        else if ((2 * M_PI - rotation) + angleReq < rotation - angleReq) {
            rotation += 0.05;
        }
        // else if (angleReq - rotation > 0.01) {
        //     rotation += 0.045;
        // } /* Moves ship counterclockwise normally */
        // else if (angleReq - rotation < -0.01) {
        //     rotation -= 0.045;  
        // }
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