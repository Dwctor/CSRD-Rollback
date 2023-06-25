//This code will be mostly C compliant except for passing by reference, which will be done with &. That is stricly because I find it more elegant.
#include <stdio.h>
#include <math.h>
#include "raylib.h"

#include "Game.h"
#include "Common.h"
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
    int counter = COUNTER_MESSAGE - 1;
    int num_rollbacks = 0;

    RBState R;

    InitGame(R);
    network_start(&nw);         // Open network connection
    // Main game loop
    while (!WindowShouldClose()){    // Detect window close button or ESC key
        //----------------------------------------------------------------------------------
        // State Update Section
        //----------------------------------------------------------------------------------

        GameLoop(R);

        //----------------------------------------------------------------------------------
        // Network Update and Rollback Section
        //----------------------------------------------------------------------------------
        
        if (++counter == COUNTER_MESSAGE) {
            counter = 0;
            send_m.S = R.S[R.CurrentFrame % RB_FRAMES];
            send_m.CurrentFrame = R.CurrentFrame;
            network_send(&nw, &send_m);
            //SendStateInMessage(R);
            while (network_get(&nw, &rec_m)) {
                //fprintf(stderr,
                //       "Received frame: %d, currently in frame: %d\n",
                //        rec_m.CurrentFrame,
                //        R.CurrentFrame);
//                fprintf(stderr,
//                        "Received adversary pos: %.2f, %.2f\n",
//                        rec_m.S.AdversaryPos.x,
//                        rec_m.S.AdversaryPos.y);
//                fprintf(stderr,
//                        "Received player pos: %.2f, %.2f\n",
//                        rec_m.S.PlayerPos.x,
//                        rec_m.S.PlayerPos.y);
                VerifyPrediction(R, rec_m);
//                ++num_rollbacks;
//                fprintf(stderr,
//                        "%d predictions verified\n",
//                        num_rollbacks);
            }
        }

        
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

