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
 * @notes: ISSUE, SONGS GET FASTER AND FASTER WHEN TOGGLING B/W PAGES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::titleScreen() 
{
    /* Creates title screen texture */
    Image titleScreen = LoadImage("./images/titleScreen.png");
    Texture2D titleScreenTexture = drawImages(titleScreen, screenWidth, screenHeight);

    /* Creates sound */
    InitAudioDevice();
    Music titleMusic = LoadMusicStream("./soundtrack/titleMusic.mp3"); 
    PlayMusicStream(titleMusic);

    /* Creates font */
    Font pirateFont = LoadFontEx("./fonts/theDarkestPearl.ttf", 200, 0, 250);

    /* Sets the button roundness */
    float roundness = 0.45f;
    int segments = 8;

    bool closeTutorial = false;

    Image instructions = LoadImage("./images/instructions.png");
    Texture2D instructionsTexture = drawImages(instructions, screenWidth - 170, screenHeight - 170);
    instructionsPopUp = false;
    bool aboutUs = false;

    while (!WindowShouldClose() && !closeTutorial && !aboutUs)    /* Detect window close button or ESC key */
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
            Rectangle playBox = drawButton(playB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.7);

            const char *settingsB = "Instructions";
            Rectangle settingsBox = drawButton(settingsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.4);

            const char *creatorsB = "Creators";
            Rectangle creatorsBox = drawButton(creatorsB, roundness, segments, pirateFont, 70, screenWidth, screenHeight, 1.2);

            /* Check if Play Button is pressed */
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), playBox)) {
                closeTutorial = true;
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), settingsBox)) {
                instructionsPopUp = !instructionsPopUp;
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), creatorsBox)) {
                aboutUs = !aboutUs;
            }

        if (instructionsPopUp) {
            instructionsPage(instructionsTexture);
        }
        EndDrawing();

        if (closeTutorial){
            tutorialScreen();
        }
        if (aboutUs) {
            aboutUsScreen();
        }
    }
    unloadTitleScreen(titleScreenTexture, pirateFont, titleMusic);
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
 * @notes: Creates a player object, NOTE: POSSIBLE BUG WHEN TOGGLING BETWEEN
 *         PLAY, RESTART, AND PAUSE (2 windows are opened)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void screen::tutorialScreen() {
    /* Condition to pause */
    bool pause = 0;
    int framesCounter = 0;

    /* Sets the button roundness */
    float roundness = 0.45f;
    int segments = 8;

    /* Setting exit pop up box condition */
    exitPopUp = false;
    toHub = false;
    bool toHome = false;
    bool restart = false;

    playerHealth = true;
    enemyOneHealth = true;
    enemyTwoHealth = true;

    /* Creates font */
    Font pirateFont = LoadFontEx("./fonts/theDarkestPearl.ttf", 200, 0, 250);

    /* Creates background texture */
    Image background = LoadImage("./images/ocean.png");
    
    ImageResize(&background, screenWidth/2, screenHeight/2);
    Texture2D texture = LoadTextureFromImage(background); 
    UnloadImage(background);

    /* Loading pause/play components */
    Image pauseBtn = LoadImage("./images/pause.png");
    Texture2D pauseTexture = drawImages(pauseBtn, 65, 68);

    Image playBtn = LoadImage("./images/playButton.png");
    Texture2D playTexture = drawImages(playBtn, 60, 68);

    Image pauseScreen = LoadImage("./images/pausePage.png");
    Texture2D pauseScreenTexture = drawImages(pauseScreen, screenWidth/1.5, screenHeight/1.3);

    /* Sets up music */
    Music tutorialMusic = LoadMusicStream("./soundtrack/tutorialMusic.mp3");
    PlayMusicStream(tutorialMusic);

    /* Set our game to run at 60 frames-per-second */
    SetTargetFPS(60);         

    float scrollingBack = 0.0f;

    /* Setting up the instructions texture */
    Image instructions = LoadImage("./images/instructions.png");
    Texture2D instructionsTexture = drawImages(instructions, screenWidth - 170, screenHeight - 170);
    instructionsPopUp = false;

    /* Create player and enemies */
    player *p1 = new player;
    enemyRed *e1 = new enemyRed;
    enemyRed *e2 = new enemyRed;
    std::vector<ship*> allShips;
    allShips.push_back(p1);
    allShips.push_back(e1);
    allShips.push_back(e2);

    while (!WindowShouldClose() && !toHub && !toHome && !restart && playerHealth && (enemyOneHealth || enemyTwoHealth))    /* Detect window close button or ESC key */
    {
        Rectangle pausePlayBtn = {screenWidth - 97, 30, 60, 65};
        if ((IsKeyPressed(KEY_P)) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), pausePlayBtn))) pause = !pause;
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

            /* Draws the pause screen */ 
            if (pause) {
                /* Darkens the screen */
                DrawRectangle(0, 0, screenWidth, screenHeight, (Color){ 0, 0, 0, 200 });
                /* Draws the pause screen */
                DrawTexture(pauseScreenTexture, screenWidth/6, 75, (Color){255,255,255,255});
                /* Draws the "paused" title */
                DrawText(TextFormat("PAUSED"), screenWidth/2.5, 180, 75, (Color){255,255,255,255});
                /* Draws the "resume" button */
                DrawText(TextFormat("RESUME"), screenWidth/2.35, 328, 55, (Color){255,255,255,255});
                /* Draws the "instructions" screen */
                DrawText(TextFormat("INSTRUCTIONS"), screenWidth/2.85, 435, 55, (Color){255,255,255,255});
                /* Draws the play button on the top right of the screen */
                DrawTexture(playTexture, screenWidth - 97, 30, (Color){255,255,255,255});

                /* Marks the different button positions */
                Rectangle resumeBox = {screenWidth/3.4, 310, 595, 80};
                Rectangle mapBox = {screenWidth/2 - 50, 550, 100, 80};
                Rectangle homeBox = {screenWidth/2 - 225, 550, 100, 80};
                Rectangle restartBox = {screenWidth/2 + 130, 555, 80, 80};
                Rectangle instructionsBox = {screenWidth/3.4, 420, 595, 80};

                /* Check if Resume Button is pressed */
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), resumeBox)) {
                    pause = !pause;
                }
                /* Checks if the map button is pressed */
                else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), mapBox)) {
                    exitPopUp = true;
                }
                /* Checks if the home button is pressed */
                else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeBox)) {
                    toHome = !toHome;
                }
                /* Checks if the restart button is pressed */
                else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), restartBox)) {
                    restart = !restart;
                }
                /* Checks if the instructions button is pressed */
                else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), instructionsBox)) {
                    instructionsPopUp = !instructionsPopUp;
                }
            }
            
            /* Note: working pause for player and background, not enemies yet */
            else if (!pause) {
                /* Draws the pause button */ 
                DrawTexture(pauseTexture, screenWidth - 100, 30, (Color){255,255,255,255});

                /* Tracks player movement */
                /* Draws player and enemy on screen */
                p1->drawShip();
                p1->monitorPlayer(allShips);
                e1->drawShip();
                e2->drawShip();
                if (p1->enteredBounds) {
                    e1->monitorEnemyRed(allShips);
                    e2->monitorEnemyRed(allShips);
                    /* Monitors the health of a ship to determine who wins */
                    playerHealth = p1->monitorShipHealth();
                    enemyOneHealth = e1->monitorShipHealth();
                    enemyTwoHealth = e2->monitorShipHealth();
                }
            }

            if (exitPopUp){
                drawPopUpWindow();
            }
            if (instructionsPopUp) {
                instructionsPage(instructionsTexture);
            }
        
        EndDrawing();
        if (!playerHealth) {
            defeatScreen();
        }
        else if (!enemyOneHealth && !enemyTwoHealth) {
            victoryScreen(e1->spawnTypes, e2->spawnTypes);
        }
        else if (toHub) {
            tortugaHubScreen();
        }
        else if (toHome) {
            titleScreen();
        }
        else if (restart) {
            tutorialScreen();
        }
    }

    unloadTutorialScreen(tutorialMusic, pauseTexture, playTexture,texture);

    p1->unloadPlayerComponents();
    e1->unloadEnemyComponents();
    e2->unloadEnemyComponents();

    delete p1;
    delete e1;
    delete e2;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: tortugaHubScreen
 * @purpose: Draws the hub screen with navigation to the market, challenges
 *           and upgrade pages
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::tortugaHubScreen()
{
    Image hubScreen = LoadImage("./images/tortugaHub.png");
    Texture2D hubScreenTexture = drawImages(hubScreen, screenWidth, screenHeight);
    bool toChallengePage = false;
    bool toMarketPage = false;
    exitPopUp = false;

    while (!WindowShouldClose() && !toMarketPage && !toChallengePage)    /* Detect window close button or ESC key */
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawTexture(hubScreenTexture, 0, 0, (Color){255, 255, 255, 255});
            Rectangle challengePage = {20, screenHeight/3.7, 360, 500};
            Rectangle upgradePage = {screenWidth/2-250, screenHeight/2.25, 430, screenHeight-screenHeight/2.25};
            Rectangle marketPage = {screenWidth-395, screenHeight/3.7, 370, 500};

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), challengePage)) {
                toChallengePage = !toChallengePage;
            }
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), upgradePage)) {
                exitPopUp = true;
            }
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), marketPage)) {
                toMarketPage = !toMarketPage;
            }

        if (exitPopUp) {
            progressPopUp();
        }

        EndDrawing();
        
        if(toChallengePage) {
            challengeScreen();
        }
        else if (toMarketPage) {
            marketScreen();
        }
    }
    
    UnloadTexture(hubScreenTexture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: challengeScreen
 * @purpose: Draws the challenges page, STILL IN PROGRESS
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::challengeScreen()
{
    Image challengePage = LoadImage("./images/challengePage.png");
    Texture2D challengePageTexture = drawImages(challengePage, screenWidth, screenHeight);
    exitPopUp = true;

    Image map = LoadImage("./images/map.png");
    Texture2D mapTexture = drawImages(map, 120, 120);

    Image home = LoadImage("./images/home.png");
    Texture2D homeTexture = drawImages(home, 100, 100);

    bool goHome = false;
    bool goMap = false;

    /* Detect window close button or ESC key */
    while (!WindowShouldClose() && !goHome && !goMap)    
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawTexture(challengePageTexture, 0, 0, (Color){255, 255, 255, 255});
            DrawText(TextFormat("CHALLENGES"), screenWidth/3, 80, 75, (Color){255,255,255,255});
            DrawTexture(mapTexture, screenWidth-170, 50, (Color){255, 255, 255, 255});
            DrawTexture(homeTexture, 50, 60, (Color){255, 255, 255, 255});
            if (exitPopUp) {
                progressPopUp();
            }
            
            Rectangle mapButton = {screenWidth-170, 50,  120,  120};
            Rectangle homeButton = {50, 60, 100, 100};

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), mapButton)) {
                goMap = true;
            }
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeButton)) {
                goHome = true;
            }

        EndDrawing();

        if (goMap) {
            tortugaHubScreen();
        }
        else if (goHome){
            titleScreen();
        }
    }
    UnloadTexture(challengePageTexture);
    UnloadTexture(mapTexture);
    UnloadTexture(homeTexture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: marketScreen
 * @purpose: Draws the market page, STILL IN PROGRESS
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::marketScreen()
{
    Image marketPage = LoadImage("./images/market.png");
    Texture2D marketPageTexture = drawImages(marketPage, screenWidth, screenHeight);
    exitPopUp = true;

    Image map = LoadImage("./images/map.png");
    Texture2D mapTexture = drawImages(map, 120, 120);

    Image home = LoadImage("./images/home.png");
    Texture2D homeTexture = drawImages(home, 100, 100);

    bool goHome = false;
    bool goMap = false;

    /* Detect window close button or ESC key */
    while (!WindowShouldClose() && !goHome && !goMap)    
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawTexture(marketPageTexture, 0, 0, (Color){255, 255, 255, 255});
            DrawText(TextFormat("MARKET"), screenWidth/3+50, 180, 100, (Color){255,255,255,255});
            DrawTexture(mapTexture, screenWidth-170, 50, (Color){255, 255, 255, 255});
            DrawTexture(homeTexture, 50, 60, (Color){255, 255, 255, 255});
            if (exitPopUp) {
                progressPopUp();
            }
            
            Rectangle mapButton = {screenWidth-170, 50,  120,  120};
            Rectangle homeButton = {50, 60, 100, 100};

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), mapButton)) {
                goMap = true;
            }
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeButton)) {
                goHome = true;
            }

        EndDrawing();

        if (goMap) {
            tortugaHubScreen();
        }
        else if (goHome){
            titleScreen();
        }
    }
    UnloadTexture(marketPageTexture);
    UnloadTexture(mapTexture);
    UnloadTexture(homeTexture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: victoryScreen
 * @purpose: Draws the market page, STILL IN PROGRESS
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::victoryScreen(int enemyOneSpawnTypes[5], int enemyTwoSpawnTypes[5])
{
    Image victoryPage = LoadImage("./images/victory.png");
    Texture2D victoryPageTexture = drawImages(victoryPage, screenWidth, screenHeight);

    toHub = false;
    bool toHome = false;
    bool restart = false;

    /* Detect window close button or ESC key */
    while (!WindowShouldClose() && !toHub && !toHome && !restart)    
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawTexture(victoryPageTexture, 0, 0, (Color){255, 255, 255, 255});

            DrawText(TextFormat("%d", (enemyOneSpawnTypes[0] + enemyTwoSpawnTypes[0])), screenWidth/2 - 340, screenHeight/2 - 50, 40, (Color){255,255,255,255});
            DrawText(TextFormat("%d", (enemyOneSpawnTypes[1] + enemyTwoSpawnTypes[1])), screenWidth/2 - 175, screenHeight/2 - 50, 40, (Color){255,255,255,255});
            DrawText(TextFormat("%d", (enemyOneSpawnTypes[2] + enemyTwoSpawnTypes[2])), screenWidth/2 + 30, screenHeight/2 - 50, 40, (Color){255,255,255,255});
            DrawText(TextFormat("%d", (enemyOneSpawnTypes[3] + enemyTwoSpawnTypes[3])), screenWidth/2 + 220, screenHeight/2 - 50, 40, (Color){255,255,255,255});
            DrawText(TextFormat("%d", (enemyOneSpawnTypes[4] + enemyTwoSpawnTypes[4])), screenWidth/2 + 385, screenHeight/2 - 50, 40, (Color){255,255,255,255});
            
            Rectangle mapBox = {screenWidth/2 - 25, screenHeight-200, 100, 80};
            Rectangle homeBox = {screenWidth/2 - 195, screenHeight-200, 100, 80};
            Rectangle restartBox = {screenWidth/2 + 160, screenHeight-200, 80, 80};

            /* Checks if the map button is pressed */
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), mapBox)) {
                toHub = !toHub;
            }
            /* Checks if the home button is pressed */
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeBox)) {
                toHome = !toHome;
            }
            /* Checks if the home button is pressed */
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), restartBox)) {
                restart = !restart;
            }

        EndDrawing();

        if (toHub) {
            tortugaHubScreen();
        }
        else if (toHome) {
            titleScreen();
        }
        else if (restart) {
            tutorialScreen();
        }
    }
    UnloadTexture(victoryPageTexture);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: defeatScreen
 * @purpose: Draws the player defeat page, STILL IN PROGRESS
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::defeatScreen()
{
    Image defeatPage = LoadImage("./images/defeat.png");
    Texture2D defeatPageTexture = drawImages(defeatPage, screenWidth, screenHeight);

    toHub = false;
    bool toHome = false;
    bool restart = false;

    /* Detect window close button or ESC key */
    while (!WindowShouldClose() && !toHub && !toHome && !restart)    
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawTexture(defeatPageTexture, 0, 0, (Color){255, 255, 255, 255});
            
            Rectangle mapBox = {screenWidth/2 - 43, screenHeight-370, 100, 80};
            Rectangle homeBox = {screenWidth/2 - 195, screenHeight-370, 100, 80};
            Rectangle restartBox = {screenWidth/2 + 120, screenHeight-370, 80, 80};

            /* Checks if the map button is pressed */
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), mapBox)) {
                toHub = !toHub;
            }
            /* Checks if the home button is pressed */
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeBox)) {
                toHome = !toHome;
            }
            /* Checks if the home button is pressed */
            else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), restartBox)) {
                restart = !restart;
            }

        EndDrawing();

        if (toHub) {
            tortugaHubScreen();
        }
        else if (toHome) {
            titleScreen();
        }
        else if (restart) {
            tutorialScreen();
        }
    }
    UnloadTexture(defeatPageTexture);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: aboutUsScreen
 * @purpose: Draws the player defeat page, STILL IN PROGRESS
 *
 * @parameters: none
 *     
 * @returns: nothing
 * @effects: Changes the background screen 
 * @notes: Creates a player object
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::aboutUsScreen()
{
    bool toHome = false;
    Rectangle background = {0, 0, screenWidth, screenHeight};
    Rectangle homeBox = {screenWidth - 100, 30, 75, 75};

    Image home = LoadImage("./images/home.png");
    Texture2D homeTexture = drawImages(home, 75, 75);

    Image alexPhoto = LoadImage("./images/alex.png");
    Texture2D alex = drawImages(alexPhoto, screenWidth/7, screenHeight/3);

    Image audreyPhoto = LoadImage("./images/audrey.png");
    Texture2D audrey = drawImages(audreyPhoto, screenWidth/6.5, screenHeight/3);

    /* Creates font */
    Font pirateFont = LoadFontEx("./fonts/theDarkestPearl.ttf", 200, 0, 250);

    futurePopUp = false;

    /* Detect window close button or ESC key */
    while (!WindowShouldClose() && !toHome)    
    { 
        BeginDrawing();
            ClearBackground((Color){0, 0, 0, 255});
            DrawRectangleRec(background, (Color){48, 48, 48, 255});
            DrawTexture(homeTexture, screenWidth - 100, 30, (Color){255, 255, 255, 255});
            DrawText(TextFormat("CREATORS"), screenWidth/3 + 50, 55, 75, (Color){255,255,255,255});
            DrawTexture(audrey, screenWidth/2 + 25, screenHeight/6, (Color){255, 255, 255, 255});
            DrawTexture(alex, screenWidth/2 - screenWidth/6.5, screenHeight/6, (Color){255, 255, 255, 255});
            DrawText(TextFormat("Hi everyone, thank you for\n  playing our game! I\'m Alex\n       and I\'m a 20 year old\n      majoring in Mechanical\n   Engineering and minoring\n        in Computer Science."), screenWidth/7.8, screenHeight/5, 20, (Color){255,255,255,255});
            DrawText(TextFormat("and I\'m Audrey (also 20)\nmajoring in Computer Science\nwith a minor in Human\nFactors Engineering. We are\nboth rising juniors at\nTufts University."), screenWidth/2 + screenWidth/5, screenHeight/5, 20, (Color){255,255,255,255});
            DrawText(TextFormat("ABOUT US: We wanted to create a video game this summer (2022) because we both wanted to challenge\nourselves. We have worked on projects before, but never to this scale. The reason we chose to make a game\nwas because we wanted our end product to be something that we or our friends could enjoy. We decided on a\npirate based game because I (Alex) recently watched Pirates of the Caribbean and pitched the idea to Audrey\nand she liked it (so simple yet it worked). Our initially naive thought process led us to believe we could\ncomplete this game by the end of the summer. Of course, as we started actually coding, designing, and drawing\n(which neither of us have ever done before), we realized it would take a lot longer than a couple months for\nour ambitious ideas. So while the game at its current state is incomplete in our eyes, we hope you will find\nit fun to play at least for the first few minutes :) "), screenWidth/3 - screenWidth/4.5, screenHeight - screenHeight/2.4, 20, (Color){255,255,255,255});

            //Rectangle futureBox = {screenWidth/5 + screenWidth/5, screenHeight/2, 250, 55};
            Rectangle futureBox = {screenWidth - 350, screenHeight - screenHeight/7, 320, 45};
            DrawRectangleRounded(futureBox, 0.45f, 8, (Color){ 0, 0, 0, 255 });
            DrawText(TextFormat("FUTURE PLANS FOR THE GAME"), futureBox.x + 30, futureBox.y + 15, 17, (Color){255,255,255,255});

            /* Checks if the home button is pressed */
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), homeBox)) {
                toHome = !toHome;
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), futureBox)) {
                futurePopUp = !futurePopUp;
            }
        
            if (futurePopUp) {
                drawFuturePopUp();
                }
        EndDrawing();
    }

    if (toHome) {
        titleScreen();
    }
    UnloadTexture(alex);
    UnloadTexture(homeTexture);
    UnloadFont(pirateFont);
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
 * @returns: a rectangle representing where the button is
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
        DrawRectangleRounded(box, roundness, segments, (Color){ 0, 0, 0, 255 });
        Vector2 textPos = {box.x + ((boxWidth - textSize.x) / 2) , box.y + ((boxHeight - textSize.y) / 2)};
        DrawTextEx(pirateFont, text, textPos, fontSize, -1, (Color){255, 255, 255, 255});

        return box;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawImages
 * @purpose: Helper function to draw textures
 *
 * @parameters: img: image to be turned into a texture for drawing
 *              width: width to resize image to
 *              height: height to resize image to
 *     
 * @returns: nothing
 * @effects: n/a
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Texture2D screen::drawImages(Image img, int width, int height)
{
    /* Loads in image and resizes it for texture */
    ImageResize(&img, width, height);
    Texture2D imgTexture = LoadTextureFromImage(img); 
    UnloadImage(img);
    return imgTexture;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: unloadTitleScreen
 * @purpose: Unloads all remaining textures after the game is over
 *
 * @parameters: currTexture: the texture to be unloaded
                currFont: the font to be unloaded
                currMusic: the music to be unloaded
 *     
 * @returns: nothing
 * @effects: Unloads a texture, font and music
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::unloadTitleScreen(Texture2D currTexture, Font currFont, Music currMusic) {
    UnloadTexture(currTexture);
    UnloadFont(currFont);
    UnloadMusicStream(currMusic);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: unloadTutorialScreen
 * @purpose: Unloads all remaining textures after the game is over
 *
 * @parameters: currTexture: the texture to be unloaded
                currFont: the font to be unloaded
                currMusic: the music to be unloaded
 *     
 * @returns: nothing
 * @effects: Unloads a texture, font and music
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::unloadTutorialScreen(Music tutorialMusic, Texture2D pauseTexture, Texture2D playTexture, Texture2D texture) {
    UnloadMusicStream(tutorialMusic);
    UnloadTexture(pauseTexture);
    UnloadTexture(playTexture);
    UnloadTexture(texture);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawPopUpWindow
 * @purpose: Draws the pop up window to ask if the user wants to continue
 *           and loose their progress
 *
 * @parameters: n/a
 *     
 * @returns: nothing
 * @effects:
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::drawPopUpWindow(){
    Rectangle popUpBox = {screenWidth/3, screenHeight/3, screenWidth/3, screenHeight/3};
    DrawRectangleRec(popUpBox, (Color){0,0,0,255});
    DrawText(TextFormat("Are you sure you want to exit? \n You will lose all the progress \n      you've made in this level."), screenWidth/3 + 50, screenHeight/3 + 50, 25, (Color){255,255,255,255});
    Rectangle cancelBox = {screenWidth/2 - 170, screenHeight/2 + 45, 150, 55};
    Rectangle continueBox = {screenWidth/2 + 30, screenHeight/2 + 45, 150, 55};
    DrawRectangleRounded(cancelBox, 0.45f, 8, (Color){255,255,255,255});
    DrawRectangleRounded(continueBox, 0.45f, 8, (Color){255,255,255,255});
    DrawText(TextFormat("CANCEL"), cancelBox.x + 40, cancelBox.y + 20, 20, (Color){0,0,0,255});
    DrawText(TextFormat("CONTINUE"), continueBox.x + 25, continueBox.y + 20, 20, (Color){0,0,0,255});
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), cancelBox)) {
        exitPopUp = false;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), continueBox)) {
        toHub = true;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: progressPopUp
 * @purpose: Draws the pop up window to show parts of the game that are
 *           still under construction
 * 
 * @parameters: n/a
 *     
 * @returns: nothing
 * @effects:
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::progressPopUp(){
    Rectangle popUpBox = {screenWidth/3, screenHeight/3, screenWidth/3, screenHeight/3};
    DrawRectangleRec(popUpBox, (Color){0,0,0,255});
    DrawText(TextFormat("  This part of the game is still\nunder construction. Check back \n     soon for more updates!"), screenWidth/3 + 50, screenHeight/3 + 50, 25, (Color){255,255,255,255});
    Rectangle notificationBox = {screenWidth/2 - 43, screenHeight/2 + 45, 100, 55};
    DrawRectangleRounded(notificationBox, 0.45f, 8, (Color){255,255,255,255});
    DrawText(TextFormat("OK!"), notificationBox.x + 35, notificationBox.y + 20, 20, (Color){0,0,0,255});
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), notificationBox)) {
        exitPopUp = false;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: progressPopUp
 * @purpose: Draws the pop up window for instructions
 * 
 * @parameters: The instructions texture to draw
 *     
 * @returns: nothing
 * @effects:
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::instructionsPage(Texture2D instructionsTexture){
    DrawTexture(instructionsTexture, 70, 70, (Color){255, 255, 255, 255});
    Rectangle exit = {screenWidth - 170, 85, 55, 55};
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), exit)) {
        instructionsPopUp = !instructionsPopUp;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @function: drawFuturePopUp
 * @purpose: Draws the pop up window to show what the plans are for the future
 * 
 * @parameters: n/a
 *     
 * @returns: nothing
 * @effects:
 * @notes: 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void screen::drawFuturePopUp(){
    Rectangle popUpBox = {screenWidth/12, screenHeight/5, screenWidth/1.2, screenHeight/1.5};
    DrawRectangleRec(popUpBox, (Color){0,0,0,255});
    DrawText(TextFormat("We still have some exciting plans for the game, including making upgrades (with cosmetic\nchanges) for the ship that the player can purchase with materials and gold they collect\nfrom defeating enemies. We have this whole idea to create a player hub known as,\nTortuga, where the player can acquire challenges and when they complete those\nchallenges they will receive gold and materials. There will also be a store to buy materials\nwith gold and a harbor to purchase upgrades and cosmetic changes for the player\'s ship.\nWe also wanted to create several levels with different enemies that spawn. The interesting\nthing is that each type of enemy would behave different and have a different way of\nattacking the player, from dropping fire barrels to ramming the player\'s ship. Even\nthough we still have more work to do in the future, we both learned a lot from this project\nthat we will take with us into our lives, whether its CS related or not. For instance, coding"), popUpBox.x + 50, popUpBox.y + 90, 25, (Color){255,255,255,255});
    Rectangle notificationBox = {screenWidth - popUpBox.x - 50, popUpBox.y, 55, 55};
    DrawText(TextFormat("in the middle of the summer without an AC is not fun for anyone unless you have popsicles."),popUpBox.x + 50, popUpBox.y + 495, 25, (Color){255,255,255,255});
    
    DrawText(TextFormat("x"), notificationBox.x, notificationBox.y, 50, (Color){255,255,255,255});
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), notificationBox)) {
        futurePopUp = false;   
    }
}