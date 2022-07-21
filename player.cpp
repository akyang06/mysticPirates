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

#define PLAYER_MAX_SHOOTS   10

typedef struct Shoot {
    Vector2 position;
    Vector2 speed;
    float radius;
    float rot;
    int lifeSpawn;
    bool active;
    Color color;
} Shoot;

static Shoot shoot[PLAYER_MAX_SHOOTS] = { 0 };


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
player::player(int screenWidth, int screenHeight) : ship(screenWidth, screenHeight) {

    acceration = 0.015;
    deceleration = 0.01;
    turnDrag = 0.005;
    drag = 0.0025;
    velLimit = 5;

    // Initialization shoot
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        shoot[i].position = (Vector2){0, 0};
        shoot[i].speed = (Vector2){0, 0};
        shoot[i].radius = 5;
        shoot[i].active = false;
        shoot[i].lifeSpawn = 0;
        shoot[i].color = (Color){ 80, 80, 80, 255 };
    }

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
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
    rotatePlayer();
    movePlayer();
    playerShoot();

    DrawText(TextFormat("rotation: %f", rotation), 20, 10, 20, (Color){255,255,255,255});
    DrawText(TextFormat("velocity x: %f", velComp.x), 20, 80, 20, (Color){255,255,255,255});
    DrawText(TextFormat("velocity y: %f", velComp.y), 20, 110, 20, (Color){255,255,255,255});
    DrawText(TextFormat("velocity mag: %f", velMag), 20, 140, 20, (Color){255,255,255,255});
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
        rotation += 0.01;
        decelerateShip(turnDrag);
    }

    if (IsKeyDown(KEY_LEFT)) {
        rotation -= 0.01;
        decelerateShip(turnDrag);
    }

    /* Keeps the rotation between 0 and 2pi radians */
    rotation = fmod(rotation + (2 * M_PI), 2 * M_PI);
}

void player::playerShoot(){
    if (IsKeyPressed(KEY_SPACE))
    {
        for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
        {
            if (!shoot[i].active)
            {
                shoot[i].position = (Vector2){ getX(), getY() };
                shoot[i].active = true;
                shoot[i].speed.x = 1.5*sin(rotation)*7;
                shoot[i].speed.y = 1.5*cos(rotation)*7;
                shoot[i].rot = rotation;
                break;
            }
        }
    }

    // Shoot life timer
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        if (shoot[i].active) shoot[i].lifeSpawn++;
    }

    // Shot logic
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        if (shoot[i].active)
        {
            // Movement
            shoot[i].position.x -= shoot[i].speed.x;
            shoot[i].position.y += shoot[i].speed.y;
            
            // Life of shoot
            if (shoot[i].lifeSpawn >= 60)
            {
                shoot[i].position = (Vector2){0, 0};
                shoot[i].speed = (Vector2){0, 0};
                shoot[i].lifeSpawn = 0;
                shoot[i].active = false;
            }
        }
    }
    // Draw shoot
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, (Color){ 80, 80, 80, 255 });
    }
}