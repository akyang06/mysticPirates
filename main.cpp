#include <iostream>
#include <fstream>
#include <string>

#include "raylib.h"
#include "rlgl.h"

typedef struct Player {
    Vector2 position;
    float speed;
} Player;

/* Program main entry point ------------------------------------------------- */
int main()
{
    /* Initialization ------------------------------------------------------- */
    InitWindow(0, 0, "Game Screen");
    MaximizeWindow();

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    
    /* Creates background texture */
    Image background = LoadImage("backgroundSheet.png");
    ImageResize(&background, screenWidth/2, screenHeight/2);
    Texture2D texture = LoadTextureFromImage(background); 
    UnloadImage(background);

    /* Creates sprite texture of player ship */
    Image sprite = LoadImage("spriteCropped.gif");
    ImageResize(&sprite, 75, 100);
    Texture2D spriteTexture = LoadTextureFromImage(sprite); 
    //UnloadImage(sprite);

    SetTargetFPS(60);         /* Set our game to run at 60 frames-per-second */

    float scrollingBack = 0.0f;
    float posX = (screenWidth / 2) - (sprite.width / 2);
    float posY = (screenHeight / 2) - (sprite.height / 2);
    float rotation = 0;
  
    // /* Player Initialization */
    // Player player = { 0 };
    // player.position = (Vector2){ 400, 280 };
    // player.speed = 0;

    /* Main game loop ------------------------------------------------------ */
    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        scrollingBack -= 1.0f;
        /* Allows texture to repeat when it gets to the end of the background */
        if (scrollingBack <= -background.width*2) {
            scrollingBack = 0;
        } 

        if (IsKeyDown(KEY_RIGHT)) {
                rotation += 5;
        }

        if (IsKeyDown(KEY_LEFT)) {
                rotation -= 5;
        }
        

        /* Drawing Section -------------------------------------------------- */
        BeginDrawing();
            

            ClearBackground((Color){255,255,255,255});

            /* Draws background */
            DrawTextureEx(texture, (Vector2){ scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});
            DrawTextureEx(texture, (Vector2){ background.width*2 + scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});
            
            /* Draws player sprite */
            DrawTextureEx(spriteTexture, (Vector2){posX, posY}, rotation, 1, (Color){255,255,255,255});
            

        EndDrawing();
    }

    /* De-Initialization----------------------------------------------------- */
    UnloadTexture(texture);             /* Texture unloading */
    UnloadTexture(spriteTexture);    
    CloseWindow(); 

    /* ---------------------------------------------------------------------- */
    return 0;
}