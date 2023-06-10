//This code will be mostly C compliant except for passing by reference, which will be done with &. That is stricly because I find it more elegant.
#include <stdio.h>
#include <math.h>
#include "raylib.h"

#include "Backend.h"
#include "Common.h"


int main(void){
    RBState R;
    MESSAGE M;
    srand(0);

    InitGame(R);

    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        //----------------------------------------------------------------------------------
        // State Update Section
        //----------------------------------------------------------------------------------

        ServerLoop(R);

        //----------------------------------------------------------------------------------
        // Network Update and Rollback Section
        //----------------------------------------------------------------------------------
        
        //SendStateInMessage(R);

        /*while(GetMessage(M)){
          VerifyPrediction(R, M);
        }*/
        
        //----------------------------------------------------------------------------------
        // Draw section
        //----------------------------------------------------------------------------------
        DrawState(R.S[R.CurrentFrame%RB_FRAMES]); 

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

