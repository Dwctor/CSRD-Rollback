#include <stdio.h>
#include "raylib.h"

#define TRAIL_S 50
#define TRAIL_R 1
#define ADV_SPD 10
#define PLR_SPD 6

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Game Prototype");

    Vector2 Player_State = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 Adversary_State = { (float)screenWidth/2 + 40, (float)screenHeight/2 + 40};
    int go_down = 1;

    Vector2 Trail [TRAIL_S];

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update local state
        //----------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) Player_State.x += PLR_SPD;
        if (IsKeyDown(KEY_LEFT)) Player_State.x -= PLR_SPD;
        if (IsKeyDown(KEY_UP)) Player_State.y -= PLR_SPD;
        if (IsKeyDown(KEY_DOWN)) Player_State.y += PLR_SPD;

        //Temporary state update for Adversary
        //----------------------------------------------------------------------------------
        
        if(Adversary_State.y < 0){
            go_down = 1;
        } else if (Adversary_State.y > screenHeight){
            go_down = -1;
        } 

        Trail[0] = Adversary_State;
        Adversary_State.y += go_down * ADV_SPD;

        for(int i = TRAIL_S - 1; i > 0; i--){
            Trail[i] = Trail[i - 1];
        }
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------
        // Draw section
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Chase the trail for points!", 10, 10, 20, DARKGRAY);

            DrawCircleV(Adversary_State, 50, (Color){ 0, 180, 48, 255 } );
            
            for(int i = 0; i < TRAIL_S; i++){
                DrawCircleV(Trail[i], 50 - (i + 1) * TRAIL_R, (Color){ (unsigned char) (0 + i * 5), (unsigned char) (180 + (i + 1)), (unsigned char) (48 + i * 5), 255 });
            }

            DrawCircleV(Player_State, 50, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
