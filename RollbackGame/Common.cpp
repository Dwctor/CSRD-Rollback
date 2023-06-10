#include <stdio.h>
#include "Common.h"
#include <math.h>

void InitGame(RBState &R){
    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Game Prototype");

    InitState(R);

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
}

void InitState(RBState &R){
    R.S[0].PlayerPos = { (float)screenWidth/2, (float)screenHeight/2 };
    R.S[0].AdversaryPos = { (float)screenWidth/2 + 40, (float)screenHeight/2 + 40};

    R.S[0].Points = 0;
    R.S[0].TextSize = TEXT_S_MIN;

    R.CurrentFrame = 0;

    printf("Initialized Game!.\n");
    fflush(stdout);
}

double EuclideanDistance(Vector2 a, Vector2 b){
    double x = b.x - a.x;
    double y = b.y - a.y;
    double dist;

    dist = pow(x, 2) + pow(y, 2);
    dist = sqrt(dist);

    return dist;
}
