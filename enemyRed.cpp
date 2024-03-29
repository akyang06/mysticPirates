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
    acceleration = 0.012;
    deceleration = 0.01;
    turnDrag = 0.005;
    drag = 0.0025;
    velLimit = 2.5;
    rotationSpeed = 0.01;
    range = 250;
   

    /* Sets up cooldown and weapons available */
    sideCannonsUnlocked = true;
    frontCannonUnlocked = false;
    fireBarrelUnlocked = false;

    sideCannonsAvailable = true;
    frontCannonAvailable = false;
    fireBarrelAvailable = false;

    sideCannonsCooldownDuration = 5;
    sideCannonsCooldown = 0;

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

    /* Loads in loot and resizes it for texture */
    int lootWidth = shipWidth/3;
    int lootHeight = shipHeight/3;

    Image cottonImage = LoadImage("images/cotton.png");
    ImageResize(&cottonImage, lootWidth, lootHeight);
    cottonTexture = LoadTextureFromImage(cottonImage); 
    UnloadImage(cottonImage);

    /*********/

    Image woodImage = LoadImage("images/wood.png");
    ImageResize(&woodImage, lootWidth, lootHeight);
    woodTexture = LoadTextureFromImage(woodImage); 
    UnloadImage(woodImage);

    /*********/

    Image ironImage = LoadImage("images/iron.png");
    ImageResize(&ironImage, lootWidth*1.2, lootHeight/1.3);
    ironTexture = LoadTextureFromImage(ironImage); 
    UnloadImage(ironImage);

    /*********/

    Image gunpowderImage = LoadImage("images/gunpowder.png");
    ImageResize(&gunpowderImage, lootWidth, lootHeight);
    gunpowderTexture = LoadTextureFromImage(gunpowderImage); 
    UnloadImage(gunpowderImage);

    /*********/

    Image drinksImage = LoadImage("images/drinks.png");
    ImageResize(&drinksImage, lootWidth, lootHeight);
    drinksTexture = LoadTextureFromImage(drinksImage); 
    UnloadImage(drinksImage);

    /*******/

    /* Source rectangle (part of the texture to use for drawing) */
    sourceRec = (Rectangle){0.0, 0.0, shipWidth, shipHeight};

    /* Destination rectangle (screen rectangle where drawing part of texture) */
    destRec.width = shipWidth;
    destRec.height = shipHeight; 

    hitBox.width = destRec.width;
    hitBox.height = destRec.height;

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
void enemyRed::monitorEnemyRed(std::vector<ship*> &allShips) {
    this->allShips = allShips;

    /* Tracks current location and positon of the player */
    storePlayerInfo();

    /* Moves enemy into the bounds after they spawn and continue to move after */
    if (!enteredBounds) {
        moveEnemyInBoundsStart();
    } else {
        monitorCollisions();
        moveEnemyRed();
        monitorCanonballs();
        monitorFirebarrel();
        monitorCoolDown(); 
        if (!isAlive) {
            destRec.x = -200;
            destRec.y = -200;
        }
        // maybe make more exact with hitbox, i am not sure how to do this
        playerRec = (Rectangle){playerPos.x - shipWidth/2, playerPos.y - shipHeight/2, shipWidth, shipHeight};
        
        //DrawRectangleRec(playerRec, (Color){ 200, 122, 255, 255 });
        if (CheckCollisionRecs(playerRec, loot) || lootExpire <= 0){
            lootPickedUp = true;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: moveEnemyRed
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
    
    /* Enemy collision movement AI */
    if (inCorner()) {
        moveEnemyOutOfCorner();
    } else if (edgeCollision) {
        moveEnemyInBounds();
    } else if (shipCollision) {
        moveEnemyAwayFromCollidingShip();
    } else {
        /* Enemy movement and attack AI*/
        if (sideCannonsAvailable) {
            if (distMag > range) {
                getInRange();
            } else {
                attackPlayer();
            }
        } else {
            circleAround();
        }
        updateVelComp(); 
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: getInRange
 * @purpose: Moves enemyShip within range of player
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy goes within range of player
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::getInRange() {
    trajectoryInRange();
    accelerateShip(acceleration);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: trajectoryInRange
 * @purpose: Determiens if the current rotation of the enemy will put it in range of the player
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: None
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::trajectoryInRange() {
    float upperAngle;
    float lowerAngle;

    /* Determines the angles that the rotation needs to be between */
    if(aboveOrBelowPlayer()) {
        upperAngle = atan2(distToPlayer.y, distToPlayer.x + range);
        lowerAngle = atan2(distToPlayer.y, distToPlayer.x + range);
    } else {
        upperAngle = atan2(distToPlayer.y - range, distToPlayer.x);
        lowerAngle = atan2(distToPlayer.y + range, distToPlayer.x);
    }
    upperAngle = fmod(upperAngle + (2 * M_PI), 2 * M_PI);
    lowerAngle = fmod(lowerAngle + (2 * M_PI), 2 * M_PI);

    /* Makes sure upper angle is larger than lower one */
    if (upperAngle < lowerAngle) {
        float temp = upperAngle;
        upperAngle = lowerAngle;
        lowerAngle = temp;
    }

    /* Determines if enemy is facing between the upper and lower angle */
    if ((upperAngle - lowerAngle) < ((2 * M_PI) - upperAngle + lowerAngle)) {
        if ((rotation >= lowerAngle) && (rotation < upperAngle)) {
            return;
        } else {
            rotateToGetInRange(upperAngle, lowerAngle);
        }
    } else {
        if (((rotation >= upperAngle) && (rotation <= 2 * M_PI)) || ((rotation >= 0) && (rotation <= lowerAngle))) {
            return;
        } else {
            rotateToGetInRange(upperAngle, lowerAngle);
        }
    }  
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: aboveOrBelowPlayer
 * @purpose: Determiens if the enemy is mostly above/below the player or to the left/right of them
 *
 * @parameters: none
 *     
 * @returns: a boolean represnting if the enemy is mostly above/below the player or not
 * @effects: None
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool enemyRed::aboveOrBelowPlayer() {
    if (abs(distToPlayer.y) >= abs(distToPlayer.x)) {
        return true;
    }
    return false;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: rotateToGetInRange
 * @purpose: Rotates the enemy to then be able to get into range to attack player
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy orientation changes
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::rotateToGetInRange(float upperAngle, float lowerAngle) {
    
    /* Gets a random angle to align with */
    float angleReq;
    if (abs(rotation - upperAngle) <= abs(rotation - lowerAngle)) {
        angleReq = upperAngle;
    } else {
        angleReq = lowerAngle;
    }
    
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
 * @function: attackPlayer
 * @purpose: Attacks the player by firing canonballs at them or rotates enemy 
 *           slightly to be perpendicular to player
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy fires cannons or rotates slightly
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::attackPlayer() {
    float shootingAngle = atan2(distToPlayer.y, distToPlayer.x);
    shootingAngle = fmod(shootingAngle + (2 * M_PI), 2 * M_PI);

    /* If angle between enemy and player is just over PI/2 then attack, otherwise rotate slightly */
    if ((abs(shootingAngle - rotation) >= (80 * (M_PI / 180))) && (abs(shootingAngle - rotation) <= (100 * (M_PI / 180)))) {
        sideCannonAttack();

         /* Determines if enemy should rotate clockwise or counter-clockwise */
        int random = rand() % 2;
        if (random == 0) {
            circleAroundCC = true;
        } else {
            circleAroundCC = false;
        }

        //cooldown = cooldownDuration;
    } else {
        rotateToAttackPlayer(shootingAngle);
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: rotateToAttackPlayer
 * @purpose: rotates the enemy so they can fire at the player at the right angle
 *
 * @parameters: a float representing the current shooting angle that the enemy has on the player
 *     
 * @returns: nothing
 * @effects: Enemy slightly rotates
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::rotateToAttackPlayer(float shootingAngle) {

    if (shootingAngle - rotation > (100 * (M_PI / 180))) {
        rotation += rotationSpeed;
        decelerateShip(turnDrag); 
    } else {
        rotation -= rotationSpeed;
        decelerateShip(turnDrag); 
    }

    if (shootingAngle - rotation < (80 * (M_PI / 180))) {
        rotation -= rotationSpeed;
        decelerateShip(turnDrag); 
    } else {
        rotation += rotationSpeed;
        decelerateShip(turnDrag); 
    }
    
    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: circleAround
 * @purpose: Has the enemy ship circle around the player after a shot has been fired
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Enemy leaves player and circles around them
 * @notes:   
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::circleAround() {

    /* Has enemy circle around the player */
    if (distMag <= range) {
        accelerateShip(acceleration);
    } else {
        if (circleAroundCC) {
            rotation -= rotationSpeed;
        } else {
            rotation+= rotationSpeed;
        }
        accelerateShip(acceleration);
        decelerateShip(turnDrag);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: unloadEnemyComponents
 * @purpose: Unloads all the textures used in the enemyRed class at the end of
 *           the level
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: None
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void enemyRed::unloadEnemyComponents(){
    UnloadTexture(shipTexture);
    UnloadTexture(cottonTexture);
    UnloadTexture(woodTexture);
    UnloadTexture(ironTexture);
    UnloadTexture(gunpowderTexture);
    UnloadTexture(drinksTexture);
}
