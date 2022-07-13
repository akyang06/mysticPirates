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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: titleScreen
 * @purpose: draws the title screen with buttons to access other parts of
 *           the game (ie. play, settings, and creator sections)
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects:
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void homepage::titleScreen() {
    /* Initializes the screen and gets the respective height and width */
    InitWindow(0, 0, "Game Screen");
    MaximizeWindow();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    /* Creates title screen texture */
    Image titleScreen = LoadImage("./images/titleScreen.png");
    ImageResize(&titleScreen, screenWidth, screenHeight);
    Texture2D titleScreenTexture = LoadTextureFromImage(titleScreen); 
    UnloadImage(titleScreen);

    /* Creates sound */
    InitAudioDevice();
    Music titleMusic = LoadMusicStream("./soundtrack/titleMusic.mp3");
    PlayMusicStream(titleMusic);

    /* Creates font */
    Font pirateFont = LoadFontEx("./fonts/theDarkestPearl.ttf", 200, 0, 250);

    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        UpdateMusicStream(titleMusic);
        BeginDrawing();
            
            ClearBackground((Color){0, 0, 0, 255});

            /* Draws background */
            DrawTexture(titleScreenTexture, 0, 0, (Color){255, 255, 255, 255});

            /* Sets the button roundness */
            float roundness = 0.45f;
            int segments = 8;

            /* Sets up text size */
            // note: return memory of pointers
            const char *mysticPirates = "The Mystic Pirates";
            Rectangle titleBox = drawButton(mysticPirates, roundness, segments, pirateFont, 100, screenWidth, screenHeight, 10);

            const char *playB = "Play";
            Rectangle playBox = drawButton(playB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 2);

            const char *settingsB = "Settings";
            Rectangle settingsBox = drawButton(settingsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.6 );

            const char *creatorsB = "Creators";
            Rectangle creatorsBox = drawButton(creatorsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.33 );

        EndDrawing();

        /* Check if Play Button is pressed */
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), playBox)) {
            unloadItems(titleScreenTexture, pirateFont, titleMusic);
            //CloseWindow();
            play test(screenWidth, screenHeight);
        }

    }

    unloadItems(titleScreenTexture, pirateFont, titleMusic);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawButton
 * @purpose: 
 *
 * @parameters: 
 *     
 * @returns: box
 * @effects:
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Rectangle homepage::drawButton(const char *text, float roundness, int segments, Font pirateFont, int fontSize, int screenWidth, int screenHeight, float divisor) {
        Vector2 textSize = MeasureTextEx(pirateFont, text, fontSize, -1);
                
        int boxWidth = textSize.x + 70;
        int boxHeight = textSize.y + 20;
        int boxX = (screenWidth / 2) - (boxWidth / 2);
        int boxY = screenHeight / divisor;

        Rectangle box = {boxX, boxY, boxWidth, boxHeight};
        
        DrawRectangleRounded(box, roundness, segments, (Color){160, 67, 54, 255});
        Vector2 textPos = {box.x + ((boxWidth - textSize.x) / 2) , box.y + ((boxHeight - textSize.y) / 2)};
        DrawTextEx(pirateFont, text, textPos, fontSize, -1, (Color){0, 0, 0, 255});

        return box;
}

void homepage::unloadItems(Texture2D titleScreenTexture, Font pirateFont, Music titleMusic) {
    UnloadTexture(titleScreenTexture);
    UnloadFont(pirateFont);
    UnloadMusicStream(titleMusic);
}
