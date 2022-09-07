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
    acceleration = 0.015;
    deceleration = 0.01;
    turnDrag = 0.005;
    drag = 0.0025;
    velLimit = 3;
    rotationSpeed = 0.015;
    range = 60;

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    /*Initial values for attacks */
    sideCannonsUnlocked = true;
    frontCannonUnlocked = true;
    fireBarrelUnlocked = true;
    frontCannonAvailable = true;
    sideCannonsAvailable = true;
    fireBarrelAvailable = true;
    shootType = 1;

    sideCannonsCooldownDuration = 5;
    sideCannonsCooldown = 0;
    frontCannonCooldownDuration = 5;
    frontCannonCooldown = 0;
    fireBarrelCooldownDuration = 5;
    fireBarrelCooldown = 0;

    /* Loads in image and resizes it for texture */
    Image sprite = LoadImage("images/starterShip.png");
    shipWidth = screenWidth / 22;
    shipHeight = screenHeight / 11;
    ImageResize(&sprite, shipWidth, shipHeight);
    ImageRotateCW(&sprite);

     /* Loads in image and resizes it for texture */
    Image barrelImage = LoadImage("images/firebarrel.png");
    int barrelWidth = shipWidth/2.5;
    int barrelHeight = shipHeight/2.5;
    ImageResize(&barrelImage, barrelWidth, barrelHeight);
    ImageRotateCCW(&barrelImage);

    barrelTexture = LoadTextureFromImage(barrelImage); 
    UnloadImage(barrelImage);

    shipTexture = LoadTextureFromImage(sprite); 
    UnloadImage(sprite);
    shipWidth = shipTexture.width;
    shipHeight = shipTexture.height;

    /* Source rectangle (part of the texture to use for drawing) */
    sourceRec = (Rectangle){0.0, 0.0, shipWidth, shipHeight};

    /* Destination rectangle (screen rectangle where drawing part of texture) */
    destRec = (Rectangle){-150, screenHeight / 2, shipWidth, shipHeight};

    /* Draws the target rectangle based on the destRec */
    if (isAlive) {
        hitBox = (Rectangle){(destRec.x / 2), (destRec.y / 2), shipWidth, shipHeight};
    }
    
    hitBox = (Rectangle){(destRec.x / 2), (destRec.y / 2), shipWidth, shipHeight};

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
    
    /* Display text for the start of each level */
    /* Note to self: need to update for when more levels are implemented */
    if (destRec.x <= screenWidth / 3) {
        const char *levelStr = "LEVEL 1";
        int levelInt = MeasureText(levelStr, 20);
        DrawText(TextFormat("LEVEL 1"), ((screenWidth / 2) - (levelInt)), ((screenHeight / 2) - 25), 50, (Color){255,255,255,255});
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
void player::monitorPlayer(std::vector<ship*> &allShips) {

    this->allShips = allShips;
    
    /* move player and deal with kinematics */
    if (!enteredBounds) {
        enterPlayer();
    } else {
        monitorCollisions();
        rotatePlayer();
        movePlayer();
        playerAttack();
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
        accelerateShip(acceleration);
    } else if (velMag > 0) {
         /* drag force */
        decelerateShip(drag);
    }

    if (IsKeyDown(KEY_DOWN)) {
        decelerateShip(deceleration);
    }
    if (!edgeCollision && !shipCollision && !inCorner()) {
        updateVelComp();
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
    /* Rotate CW */
    if (IsKeyDown(KEY_RIGHT)) {
        rotation += rotationSpeed;
        decelerateShip(turnDrag);
    }

    /* Rotate CCW */
    if (IsKeyDown(KEY_LEFT)) {
        rotation -= rotationSpeed;
        decelerateShip(turnDrag);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: playerAttack
 * @purpose: Calls other functions to deal with player attacks
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: None
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::playerAttack(){
    attackType();
    monitorPlayerAttack();

    monitorCanonballs();
    monitorFirebarrel();
    monitorCoolDown();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: attackType
 * @purpose: Sets the attack type based on the user's choice (1 = shoot cannons 
 *           from the front, 2 = shoot cannons from the sides, 3 = leave bombs)
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: Changes the attack type for the player
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::attackType(){
    if (IsKeyPressed(KEY_ONE) && sideCannonsUnlocked){
        shootType = 1;
    }
    else if (IsKeyPressed(KEY_TWO) && frontCannonUnlocked){
        shootType = 2;
    }
    else if (IsKeyPressed(KEY_THREE) && fireBarrelUnlocked){
        shootType = 3;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: monitorPlayerAttack
 * @purpose: Monitors if the player presses the spacebar to attack and calls the respective attack function
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: None
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::monitorPlayerAttack() {
    if (IsKeyPressed(KEY_SPACE)) { 
        if (shootType == 1 && sideCannonsAvailable) {
            sideCannonAttack();
        } else if (shootType == 2 && frontCannonAvailable) {
            frontCannonAttack();
        } else if (shootType == 3 && fireBarrelAvailable) {
            fireBarrelAttack();
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: unloadPlayerComponents
 * @purpose: Unloads all the textures used in the player class at the end of
 *           the level
 *
 * @parameters: none
 *     
 * @returns: Nothing
 * @effects: None
 * @notes: n/a
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void player::unloadPlayerComponents(){
    UnloadTexture(shipTexture);
    UnloadTexture(barrelTexture);
}