//This code will be mostly C compliant except for passing by reference, which will be done with &. That is stricly because I find it more elegant.
#include <stdio.h>
#include <math.h>
#include "raylib.h"

#include "Backend.h"
#include "Common.h"
#include "message.h"
#include "../NetworkLib/network.h"
#include "message.h"

int main(int argc, char **argv) {
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

    RBState R;
    MESSAGE M;
    srand(0);

    InitGame(R);
    network_start(&nw);         // Open network connection
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
        if (++counter == 3) {
            counter = 0;
            send_m.S = R.S[0] ;
            send_m.CurrentFrame = R.CurrentFrame;
            network_send(&nw, &send_m);
        }
//        /*while(GetMessage(M)){
//          VerifyPrediction(R, M);
//        }*/
//        if (network_get(&nw, &rec_m)) {
//            VerifyPrediction(R, rec_m);
//        }

        
        //----------------------------------------------------------------------------------
        // Draw section
        //----------------------------------------------------------------------------------
        DrawState(R.S[R.CurrentFrame%RB_FRAMES]); 

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    network_end(&nw);     // Close network connection
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

