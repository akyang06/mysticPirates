/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  play.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  07/07/22
 *
 *  
 *
 *  Function implementations of play class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "play.h"

play::play(int screenWidth, int screenHeight) {
    screenW = screenWidth;
    screenH = screenHeight;

    playScreen();
}

play::~play() {

}

void play::playScreen() {
    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        BeginDrawing();
            /* Initialization ------------------------------------------------------- */
    InitWindow(screenW, screenH, "Game Screen");

    /* Creates background texture */
    Image background = LoadImage("./images/backgroundSheet.png");
    ImageResize(&background, screenW/2, screenH/2);
    Texture2D texture = LoadTextureFromImage(background); 
    UnloadImage(background);

    /* Creates sprite texture of player ship */
    Image sprite = LoadImage("./images/pirateShip.png");
    ImageResize(&sprite, 75, 100);
    ImageRotateCW(&sprite);

    Texture2D spriteTexture = LoadTextureFromImage(sprite); 
    UnloadImage(sprite);
    int spriteWidth = spriteTexture.width;
    int spriteHeight = spriteTexture.height;

    /* Source rectangle (part of the texture to use for drawing) */
    Rectangle sourceRec = {0.0f, 0.0f, (float)spriteWidth, (float)spriteHeight};

    /* Destination rectangle (screen rectangle where drawing part of texture) */
    Rectangle destRec = {screenW / 2, screenH / 2, spriteWidth, spriteHeight};

    /* Origin of the texture (rotation/scale point)
       it's relative to destination rectangle size */
    Vector2 origin = {(float) spriteWidth / 2, (float)spriteHeight / 2};

    float rotation = 0;


    SetTargetFPS(60);         /* Set our game to run at 60 frames-per-second */

    float scrollingBack = 0.0f;
    float velMag = 0;
  
    /* Main game loop ------------------------------------------------------ */
    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        scrollingBack -= 1.0f;
        /* Allows texture to repeat when it gets to the end of the background */
        if (scrollingBack <= -background.width*2) {
            scrollingBack = 0;
        } 

        /* Note for later: if rotation hits a limit, reset */
        if (IsKeyDown(KEY_RIGHT)) {
                rotation += 0.01;
                // if (velMag > 0) {
                //     velMag -= 0.01;
                // }
        }

        if (IsKeyDown(KEY_LEFT)) {
                rotation -= 0.01;
                // if (velMag > 0) {
                //     velMag -= 0.01;
                // }
        }

        if (IsKeyDown(KEY_UP)) {
                velMag += 0.005;
        }
        // } else if (velMag > 0) {
        //     /* drag force */ velMag -= 0.01;
        // }

        if (IsKeyDown(KEY_DOWN) && velMag > 0) {
                velMag -= 0.0025;
        }
        
        Vector2 velComp = {(float) cosf(rotation) * velMag, (float) sinf(rotation) * velMag};
        destRec.x += velComp.x;
        destRec.y += velComp.y;

        /* Drawing Section -------------------------------------------------- */
        BeginDrawing();
            
            ClearBackground((Color){255,255,255,255});

            /* Draws background */
            DrawTextureEx(texture, (Vector2){ scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});
            DrawTextureEx(texture, (Vector2){ background.width*2 + scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});
            
            /* Draws player sprite */
            DrawTexturePro(spriteTexture, sourceRec, destRec, origin, rotation * 57.3, (Color){255,255,255,255});
            DrawText(TextFormat("rotation: %f", rotation), 20, 20, 20, (Color){255,255,255,255});
            DrawText(TextFormat("velocity x: %f", velComp.x), 20, 50, 20, (Color){255,255,255,255});
            DrawText(TextFormat("velocity y: %f", velComp.y), 20, 80, 20, (Color){255,255,255,255});
            DrawText(TextFormat("velocity mag: %f", velMag), 20, 110, 20, (Color){255,255,255,255});
            DrawText(TextFormat("position x: %f", destRec.x), 20, 140, 20, (Color){255,255,255,255});
            DrawText(TextFormat("position y: %f", destRec.y), 20, 170, 20, (Color){255,255,255,255});
            
            // DrawLine(destRec.x, destRec.y, destRec.x + cosf(rotation)*100, destRec.y + sinf(rotation)*100, (Color){255,255,255,255});

        EndDrawing();
    }

    /* De-Initialization----------------------------------------------------- */
    UnloadTexture(texture);             /* Texture unloading */
    UnloadTexture(spriteTexture);    
    CloseWindow(); 

    }
}