//This code will be mostly C compliant except for passing by reference, which will be done with &. That is stricly because I find it more elegant.
#include <stdio.h>
#include "raylib.h"

// This game prototype will only simulate the chaser side, called Player. In the final game we need both a Player and a Adversary point of view.

// These defines quickly tweak the behaviour of the game. In this short example we keep them here, but in the final project they should be moved to their own header.
#define TRAIL_S 50
#define TRAIL_R 1
#define ADV_SPD 10
#define PLR_SPD 6

const int screenWidth = 1200;
const int screenHeight = 720;

// The full Game State should be kept in a singular struct that will always be passed by reference to functions.
// This will simplify development, parameter passing and the serialization of the game state.
typedef struct GameState {
    // Will be in the final game
    Vector2 PlayerPos;
    Vector2 AdversaryPos;
    Vector2 Trail [TRAIL_S];

    // Wont be in the final game.
    int GoDown;
} GameState;

//Updates the Player's position given input on the arrow keys.
void UpdatePlayerPosInput(GameState &S);

//In the final project this should be updated using the network. Here I am goint to move it in an arbitrary way.
void UpdateAdversaryPosArbitrary(GameState &S);

//Makes the trail "trail" behind the Adversary node.
void UpdateTrailPos(GameState &S);

//Will draw a single frame of the game.
void DrawState(GameState &S);

//Initializes the full game. (Includes game window size, state, framerate and others.)
void InitGame(GameState &S);

//Runs the main game loop (the state update part), which includes updating all positions.
void GameLoop(GameState &S);

int main(void){
    GameState S;

    InitGame(S);

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        //----------------------------------------------------------------------------------
        // State Update Section
        //----------------------------------------------------------------------------------

        GameLoop(S);

        //----------------------------------------------------------------------------------
        // Network Update Section (Blank in the prototype)
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // Draw section
        //----------------------------------------------------------------------------------

        DrawState(S); 

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void InitGame(GameState &S){
    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Game Prototype");

    S.PlayerPos = { (float)screenWidth/2, (float)screenHeight/2 };
    S.AdversaryPos = { (float)screenWidth/2 + 40, (float)screenHeight/2 + 40};

    S.GoDown = 1;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void GameLoop(GameState &S){
    UpdatePlayerPosInput(S);
    UpdateTrailPos(S);
    UpdateAdversaryPosArbitrary(S);
}
void UpdatePlayerPosInput(GameState &S){
    if (IsKeyDown(KEY_RIGHT)) S.PlayerPos.x += PLR_SPD;
    if (IsKeyDown(KEY_LEFT)) S.PlayerPos.x -= PLR_SPD;
    if (IsKeyDown(KEY_UP)) S.PlayerPos.y -= PLR_SPD;
    if (IsKeyDown(KEY_DOWN)) S.PlayerPos.y += PLR_SPD;
}
void UpdateAdversaryPosArbitrary(GameState &S){
    if(S.AdversaryPos.y < 0){
        S.GoDown = 1;
    } else if (S.AdversaryPos.y > screenHeight){
        S.GoDown = -1;
    } 
    S.AdversaryPos.y += S.GoDown * ADV_SPD;
}
void UpdateTrailPos(GameState &S){
    S.Trail[0] = S.AdversaryPos;

    for(int i = TRAIL_S - 1; i > 0; i--){
        S.Trail[i] = S.Trail[i - 1];
    }
}

void DrawState(GameState &S){
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("Chase the trail for points!", 10, 10, 20, DARKGRAY);

    DrawCircleV(S.AdversaryPos, 50, (Color){ 0, 180, 48, 255 } );
    
    for(int i = 0; i < TRAIL_S; i++){
        DrawCircleV(S.Trail[i], 50 - (i + 1) * TRAIL_R, (Color){ (unsigned char) (0 + i * 5), (unsigned char) (180 + (i + 1)), (unsigned char) (48 + i * 5), 255 });
    }

    DrawCircleV(S.PlayerPos, 50, MAROON);

    EndDrawing();
}
