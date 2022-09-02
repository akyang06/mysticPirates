/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  screen.cpp
 *  Authors: Alex Savic & Audrey Yang
 *  06/28/22
 *
 *  
 *
 *  Function implementations of screen class
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "screen.h"
#include <vector>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: constructor
 * @purpose: Initializes a maximized window and gets the dimensions of it
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: sets the values of screenWidth and screenHeight
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
screen::screen() {
    /* Initializes the screen and gets the respective height and width */
    InitWindow(0, 0, "Game Screen");
    MaximizeWindow();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    SetTargetFPS(60);         /* Set our game to run at 60 frames-per-second */
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
screen::~screen() {

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
void screen::titleScreen() {

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

    /* Sets the button roundness */
    float roundness = 0.45f;
    int segments = 8;

    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        UpdateMusicStream(titleMusic);
        BeginDrawing();
            
            ClearBackground((Color){0, 0, 0, 255});

            /* Draws background */
            DrawTexture(titleScreenTexture, 0, 0, (Color){255, 255, 255, 255});

            /* Sets up text size */
            // note: return memory of pointers
            const char *mysticPirates = "The Mystic Pirates";
            Rectangle titleBox = drawButton(mysticPirates, roundness, segments, pirateFont, 100, screenWidth, screenHeight, 10);

            const char *playB = "Play";
            Rectangle playBox = drawButton(playB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.6);

            const char *settingsB = "Settings";
            Rectangle settingsBox = drawButton(settingsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.35 );

            const char *creatorsB = "Creators";
            Rectangle creatorsBox = drawButton(creatorsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.158 );

            // const char *creatorsB = "Map";
            // Rectangle creatorsBox = drawButton(creatorsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.158 );

        EndDrawing();

        /* Check if Play Button is pressed */
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), playBox)) {
            unloadItems(titleScreenTexture, pirateFont, titleMusic);
            tutorialScreen();
            CloseWindow();
        }

    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: tutorialScreen
 * @purpose: Draws the tutorial screen with the initial player ship and teaches 
 *          the player the starting controls to the game
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void screen::tutorialScreen() {
    /* Condition to pause */
    bool pause = 0;
    int framesCounter = 0;

    /* Creates background texture */
    Image background = LoadImage("./images/ocean1.png");
    ImageResize(&background, screenWidth/2, screenHeight/2);
    Texture2D texture = LoadTextureFromImage(background); 
    UnloadImage(background);

    /* Loading menu */
    Texture2D mapTexture = drawMenu();

    /* Sets up music */
    Music tutorialMusic = LoadMusicStream("./soundtrack/tutorialMusic.mp3");
    PlayMusicStream(tutorialMusic);

    /* Set our game to run at 60 frames-per-second */
    SetTargetFPS(60);         

    float scrollingBack = 0.0f;

    /* Create player and enemies */
    player *p1 = new player;
    enemyRed *e1 = new enemyRed;
    enemyRed *e2 = new enemyRed;
    std::vector<ship*> allShips;
    allShips.push_back(p1);
    allShips.push_back(e1);
    allShips.push_back(e2);

    while (!WindowShouldClose())    /* Detect window close button or ESC key */
    {
        if (IsKeyPressed(KEY_P)) pause = !pause;
        if (!pause) {
            UpdateMusicStream(tutorialMusic);
            scrollingBack -= 1.0f;
            /* Allows texture to repeat when it gets to the end of the background */
            if (scrollingBack <= -background.width * 2) {
                scrollingBack = 0;
            } 
        }
        else framesCounter++;

        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});

            /* Draws background */
            DrawTextureEx(texture, (Vector2){ scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});
            DrawTextureEx(texture, (Vector2){ background.width*2 + scrollingBack, 0 }, 0.0f, 2.0f, (Color){255,255,255,255});

            /* Draws the menu for navigating between pages*/
            DrawTexture(mapTexture, screenWidth - 100, 30, (Color){255,255,255,255});
            
            if (pause && ((framesCounter/30)%2)) DrawText("PAUSED", 350, 200, 30, (Color){ 130, 130, 130, 255 });
            
            /* Note: working pause for player and background, not enemies yet */
            if (!pause) {
                /* Tracks player movement */
                /* Draws player and enemy on screen */
                p1->drawShip();
                p1->monitorPlayer(allShips);
                e1->drawShip();
                e2->drawShip();
                if (p1->enteredBounds) {
                    e1->monitorEnemyRed(allShips);
                    e2->monitorEnemyRed(allShips);
                }
            }
        EndDrawing();
        }
        
    delete p1;
    delete e1;
    delete e2;

    UnloadMusicStream(tutorialMusic);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawButton
 * @purpose: Draws buttons on the screen that the user can press to navigate through the game
 *
 * @parameters: text: a pointer to a character representing the text to be drawn on the screen
                roundness: an integer represnting the roundness of the boxes to be drawn
                segments: an integer representing the amount of segments to use to draw the boxes
                pirateFont: the font to be used to draw the text on the screen
                screenWidth: the screen width of the monitor
                screenHeight: the screen height of the monitor
                divisor: a intger that dictates where on the screen the box will be drawn (lower number means the box is lower on the screen)
 *     
 * @returns: a rectanglw representing where the button is
 * @effects: Draws a rectangle on screen along with some text on it
 * @notes:
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Rectangle screen::drawButton(const char *text, float roundness, int segments, Font pirateFont, int fontSize, int screenWidth, int screenHeight, float divisor) {
        /* Gets the size of the text to be drawn */
        Vector2 textSize = MeasureTextEx(pirateFont, text, fontSize, -1);
        
        /* Creates a box based on the text size and divisor */
        int boxWidth = textSize.x + 70;
        int boxHeight = textSize.y + 20;
        int boxX = (screenWidth / 2) - (boxWidth / 2);
        int boxY = screenHeight / divisor;

        Rectangle box = {boxX, boxY, boxWidth, boxHeight};
        
        /* Draws box and text on screen */
        DrawRectangleRounded(box, roundness, segments, (Color){ 94, 14, 14, 255 });
        Vector2 textPos = {box.x + ((boxWidth - textSize.x) / 2) , box.y + ((boxHeight - textSize.y) / 2)};
        DrawTextEx(pirateFont, text, textPos, fontSize, -1, (Color){255, 255, 255, 255});

        return box;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: unloadItems
 * @purpose: Draws the tutorial screen with the initial player ship and teaches 
 *          the player the starting controls to the game
 *
 * @parameters: currTexture: the texture to be unloaded
                currFont: the font to be unloaded
                currMusic: the music to be unloaded
 *     
 * @returns: nothing
 * @effects: Unloads a texture, font and music
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::unloadItems(Texture2D currTexture, Font currFont, Music currMusic) {
    //unloadComponents();
    UnloadTexture(currTexture);
    UnloadFont(currFont);
    UnloadMusicStream(currMusic);
}

Texture2D screen::drawMenu()
{
    /* Loads in image and resizes it for texture */
    Image map = LoadImage("./images/map.png");
    Texture2D mapTexture = LoadTextureFromImage(map); 
    UnloadImage(map);
    return mapTexture;
}