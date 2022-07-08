/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  homepage.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  06/28/22
 *
 *  
 *
 *  Function implementations of homepage class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "homepage.h"

homepage::homepage() {
    titleScreen();
}

homepage::~homepage() {

}

// bool homepage::GuiButton(Rectangle bounds, const char *text, int forcedState)
// // Gui button (immediate mode, update and draw)
// {
//     static const int textColor[4] = { 0xeff6ffff, 0x78e782ff, 0xb04d5fff, 0xd6d6d6ff };

//     int state = (forcedState >= 0)? forcedState : 0;                // NORMAL
//     bool pressed = false;
//     Vector2 textSize = MeasureTextEx(font, text, font.baseSize, 1);

//     // Update control
//     //--------------------------------------------------------------------
//     if ((state < 3) && (forcedState < 0))
//     {
//         Vector2 mousePoint = GetMousePosition();

//         // Check button state
//         if (CheckCollisionPointRec(mousePoint, bounds))
//         {
//             if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = 2;    // PRESSED
//             else state = 1;                                         // FOCUSED

//             if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP))
//             {
//                 pressed = true;
//                 PlaySound(fxCoin);
//             }
//         }
//     }

//     npInfo.source.x = 80*state;

//     //--------------------------------------------------------------------

//     // Draw control
//     //--------------------------------------------------------------------
//     //DrawRectangleRec(bounds, GREEN);
//     //DrawRectangleLinesEx(bounds, 4, DARKGREEN);
//     DrawTextureNPatch(texNPatch, npInfo, bounds, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
//     DrawTextEx(font, text, (Vector2){ bounds.x + bounds.width/2 - textSize.x/2, bounds.y + bounds.height/2 - textSize.y/2 + 4 }, font.baseSize, 1, GetColor(textColor[state]));
//     //------------------------------------------------------------------

//     return pressed;
// }

void homepage::titleScreen() {
    InitWindow(0, 0, "Game Screen");
    MaximizeWindow();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    /* Creates title screen texture */
    Image titleScreen = LoadImage("titleScreen.png");
    ImageResize(&titleScreen, screenWidth, screenHeight);
    Texture2D titleScreenTexture = LoadTextureFromImage(titleScreen); 
    UnloadImage(titleScreen);

    /* Creates sound */
    InitAudioDevice();
    Music titleMusic = LoadMusicStream("titleMusic.mp3");
    PlayMusicStream(titleMusic);

    /* Creates font */
    Font pirateFont = LoadFontEx("theDarkestPearl.ttf", 200, 0, 250);

    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        UpdateMusicStream(titleMusic);
        BeginDrawing();
            
            ClearBackground((Color){0, 0, 0, 255});

            /* Draws background */
            DrawTexture(titleScreenTexture, 0, 0, (Color){255, 255, 255, 255});

            /* Draws boxes and text */
            int boxWidth = 300;
            int boxHeight = 100;
            Rectangle box = {screenWidth / 5, screenHeight / 10, boxWidth, boxHeight};
            float roundness = 0.45f;
            int segments = 8;

            /* Draws title font and gets size of the text */
            Vector2 textSize = MeasureTextEx(pirateFont, "The Mystic Pirates", 100, -1);
            box.height +=  (textSize.y - box.height) + 20;
            box.width += (textSize.x - box.width) + 70;
            box.x = (screenWidth / 2) - (box.width / 2);
            DrawRectangleRounded(box, roundness , segments, (Color){160, 67, 54, 255});
            DrawTextEx(pirateFont, "The Mystic Pirates", (Vector2) {box.x + ((box.width - textSize.x) / 2) , box.y + ((box.height - textSize.y) / 2)}, 100, -1, (Color){0, 0, 0, 255});

            textSize = MeasureTextEx(pirateFont, "Play", 70, -1);
            box.y = screenHeight / 2;
            box.height +=  (textSize.y - box.height) + 20;
            box.width += (textSize.x - box.width) + 70;
            box.x = (screenWidth / 2) - (box.width / 2);
            DrawRectangleRounded(box, roundness , segments, (Color){160, 67, 54, 255});
            DrawTextEx(pirateFont, "Play", (Vector2) {box.x + ((box.width - textSize.x) / 2) , box.y + ((box.height - textSize.y) / 2)}, 70, -1, (Color){0, 0, 0, 255});

            textSize = MeasureTextEx(pirateFont, "Settings", 70, -1);
            box.y += 150;
            box.width += (textSize.x - box.width) + 70;
            box.x = (screenWidth / 2) - (box.width / 2);
            DrawRectangleRounded(box, roundness , segments, (Color){160, 67, 54, 255});
            DrawTextEx(pirateFont, "Settings", (Vector2) {box.x + ((box.width - textSize.x) / 2) , box.y + ((box.height - textSize.y) / 2)}, 70, -1, (Color){0, 0, 0, 255});

            textSize = MeasureTextEx(pirateFont, "Creators", 70, -1);
            box.y += 150;
            box.width += (textSize.x - box.width) + 70;
            box.x = (screenWidth / 2) - (box.width / 2);
            DrawRectangleRounded(box, roundness , segments, (Color){160, 67, 54, 255});
            DrawTextEx(pirateFont, "Creators", (Vector2) {(box.x + ((box.width - textSize.x) / 2)) , box.y + ((box.height - textSize.y) / 2)}, 70, -1, (Color){0, 0, 0, 255});
                
        EndDrawing();


        /* Check if Play Button is pressed */
        if(IsMouseButtonPressed(int button) && CheckCollisionPointRec(Vector2 point, Rectangle rec)) {

        }
            
    

    }



    UnloadTexture(titleScreenTexture);
    UnloadFont(pirateFont);
    UnloadMusicStream(titleMusic);

}