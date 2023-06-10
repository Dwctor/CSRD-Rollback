//This code will be mostly C compliant except for passing by reference, which will be done with &. That is stricly because I find it more elegant.
#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "network.h"

// This game prototype will only simulate the chaser side, called Player. In the final game we need both a Player and a Adversary point of view.

// These defines quickly tweak the behaviour of the game. In this short example we keep them here, but in the final project they should be moved to their own header.
#define TRAIL_S 50
#define TRAIL_R 1
#define ADV_SPD 10
#define PLR_SPD 8
#define POINTSRANGE 55
#define TEXT_S_MAX 80
#define TEXT_S_MIN 40

const int screenWidth = 1200;
const int screenHeight = 720;

// The full Game State should be kept in a singular struct that will always be passed by reference to functions.
// This will simplify development, parameter passing and the serialization of the game state.
typedef struct GameState {
    // Will be in the final game
    Vector2 PlayerPos;
    Vector2 AdversaryPos;
    Vector2 Trail [TRAIL_S];
    
    long Points;

    // Is client-sided (doesn't go to network)
    int TextSize;

    // Wont be in the final game.
    int GoDown;
    int GoRight;
} GameState;

//Updates the Player's position given input on the arrow keys.
void UpdatePlayerPosInput(GameState &S);

//In the final project this should be updated using the network. Here I am goint to move it in an arbitrary way.
void UpdateAdversaryPosArbitrary(GameState &S);

//Makes the trail "trail" behind the Adversary node.
void UpdateTrailPos(GameState &S);

//Updates the points of the player. Incremented when the player is touching the tail of the trail of the adversary.
void UpdatePointsCount(GameState &S);

//Will draw a single frame of the game.
void DrawState(GameState &S);

//Initializes the full game. (Includes game window size, state, framerate and others.)
void InitGame(GameState &S);

//Runs the main game loop (the state update part), which includes updating all positions.
void GameLoop(GameState &S);

//Returns the euclidean distance of two Vector2 points.
double EuclideanDistance(Vector2 a, Vector2 b);

int main(int argc, char **argv){
    if (argc != 3) {
        fprintf(stderr,
                "client and target ports must be parameterized: "
                "./game <client port> <target port>");
    }
    int rec_port = atoi(argv[1]);
    int send_port = atoi(argv[2]);
    struct network nw = new_network(rec_port, send_port);
    struct MESSAGE rec_m, send_m;
    int counter = 0;
    GameState S;

    network_start(&nw);
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
    network_end(&nw);
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

    S.GoDown = 1; S.GoRight = 1;
    S.Points = 0;
    S.TextSize = TEXT_S_MIN;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void GameLoop(GameState &S){
    UpdatePlayerPosInput(S);
    UpdateTrailPos(S);
    UpdateAdversaryPosArbitrary(S);

    UpdatePointsCount(S);
}
void UpdatePlayerPosInput(GameState &S){
    if (IsKeyDown(KEY_RIGHT)) S.PlayerPos.x += PLR_SPD;
    if (IsKeyDown(KEY_LEFT)) S.PlayerPos.x -= PLR_SPD;
    if (IsKeyDown(KEY_UP)) S.PlayerPos.y -= PLR_SPD;
    if (IsKeyDown(KEY_DOWN)) S.PlayerPos.y += PLR_SPD;
}
void UpdateAdversaryPosArbitrary(GameState &S){
    if(S.AdversaryPos.x < 0){
        S.GoRight = 1;
    } else if (S.AdversaryPos.x > screenWidth){
        S.GoRight = -1;
    } 
    if(S.AdversaryPos.y < 0){
        S.GoDown = 1;
    } else if (S.AdversaryPos.y > screenHeight){
        S.GoDown = -1;
    } 
    S.AdversaryPos.x += S.GoRight * ADV_SPD;
    S.AdversaryPos.y += S.GoDown * ADV_SPD;
}
void UpdateTrailPos(GameState &S){
    S.Trail[0] = S.AdversaryPos;

    for(int i = TRAIL_S - 1; i > 0; i--){
        S.Trail[i] = S.Trail[i - 1];
    }
}

void UpdatePointsCount(GameState &S){
    // Here TRAIL_S - 5 was chosen arbitrarily as the anchor to count points from.
    if(EuclideanDistance(S.PlayerPos, S.Trail[TRAIL_S - 5]) < POINTSRANGE){
        S.Points++;
        if(S.TextSize < TEXT_S_MAX) S.TextSize += 4;
        return;
    }
    if(S.TextSize > TEXT_S_MIN) S.TextSize -= 2;
    return;
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

    DrawText(TextFormat("%d", S.Points), screenWidth / 2 - MeasureText(TextFormat("%d", S.Points), S.TextSize) / 2, 20, S.TextSize, RED);

    EndDrawing();
}

double EuclideanDistance(Vector2 a, Vector2 b){
    double x = b.x - a.x;
    double y = b.y - a.y;
    double dist;

    dist = pow(x, 2) + pow(y, 2);
    dist = sqrt(dist);

    return dist;
}
