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
 * @function: moveEnemyInBoundsStart
 * @purpose: Move the enemy in bounds when they spawn into the game
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: 
 * @notes: after spawn, moves enemy into the bounds
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::moveEnemyInBoundsStart() {
    facePlayer();
    accelerateShip(acceration);
    velComp = (Vector2) {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: moveEnemyInBounds
 * @purpose: Move the enemy in bounds when are on the edge of the boundaries
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: 
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::moveEnemyInBounds() {
    /* left boundary */
    if (getX() <= leftBounds) {
        if(rotation > M_PI) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* right boundary */
    if (getX() >= rightBounds) {
        if(rotation > 3 * M_PI_2) {
            rotation -= rotationSpeed;
        } else {
            rotation += rotationSpeed;
        }
    }

    /* up boundary */
    if (getY() <= upBounds) {
        if(rotation > 3 * M_PI_2) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* down boundary */
    if (getY() >= downBounds) {
        if(rotation > M_PI_2) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* Keeps the speed of the enemy at 40% of top speed when trying to get out back in bounds */
    if (velMag <= 0.4 * velLimit) {
        accelerateShip(acceration);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: moveEnemyOutOfCorner
 * @purpose: Move the enemy out of the corner
 *
 * @parameters: none
 *     
 * @returns: none
 * @effects: Rotates enemy so the face out of the corner and into bounds
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemy::moveEnemyOutOfCorner() {
    /* top left corner */
    if (getX() <= leftBounds && getY() <= upBounds) {
        if(rotation > 5 * M_PI_4) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* top right corner */
    if (getX() >= rightBounds && getY() <= upBounds) {
        if(rotation > 7 * M_PI_4) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* bottom left corner */
    if (getX() <= leftBounds && getY() >= downBounds) {
        if(rotation > 3 * M_PI_4) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* bottom right corner */
    if (getX() >= rightBounds && getY() >= downBounds) {
        if(rotation > M_PI_4) {
            rotation += rotationSpeed;
        } else {
            rotation -= rotationSpeed;
        }
    }

    /* Keeps the speed of the enemy at 40% of top speed when trying to get out back in bounds */
    if (velMag <= 0.4 * velLimit) {
        accelerateShip(acceration);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

void enemy::storePlayerInfo() {
    playerPos = (Vector2) {allShips.front()->getX(), allShips.front()->getY()};
    distToPlayer = (Vector2) {allShips.front()->getX() - getX(), allShips.front()->getY() - getY()};
    distMag = sqrt(pow(distToPlayer.x, 2) + pow(distToPlayer.y, 2));
    playerRotation = allShips.front()->getRotation();
}