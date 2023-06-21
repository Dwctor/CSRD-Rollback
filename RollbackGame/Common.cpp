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

void CopyLastState(RBState &R){
  int RBF = R.CurrentFrame%60;
  if(RBF == 0){
    R.S[RBF] = R.S[RB_FRAMES - 1];
    return;
  }
  R.S[RBF] = R.S[RBF - 1];
}

void UpdatePlayerPos(RBState &R){
  int RBF = R.CurrentFrame%60;
  R.S[RBF].PlayerPos.x += R.S[RBF].PlayerInput.x * PLR_SPD;
  R.S[RBF].PlayerPos.y += R.S[RBF].PlayerInput.y * PLR_SPD;

  if(R.S[RBF].PlayerPos.x < 0) R.S[RBF].PlayerPos.x = 0;
  if(R.S[RBF].PlayerPos.x > screenWidth) R.S[RBF].PlayerPos.x = screenWidth;
  if(R.S[RBF].PlayerPos.y < 0) R.S[RBF].PlayerPos.y = 0;
  if(R.S[RBF].PlayerPos.y > screenHeight) R.S[RBF].PlayerPos.y = screenHeight;
} 

void UpdateAdversaryPos(RBState &R){
  int RBF = R.CurrentFrame%60;
  R.S[RBF].AdversaryPos.x += R.S[RBF].AdversaryInput.x * PLR_SPD;
  R.S[RBF].AdversaryPos.y += R.S[RBF].AdversaryInput.y * PLR_SPD;

  if(R.S[RBF].AdversaryPos.x < 0) R.S[RBF].AdversaryPos.x = 0;
  if(R.S[RBF].AdversaryPos.x > screenWidth) R.S[RBF].AdversaryPos.x = screenWidth;
  if(R.S[RBF].AdversaryPos.y < 0) R.S[RBF].AdversaryPos.y = 0;
  if(R.S[RBF].AdversaryPos.y > screenHeight) R.S[RBF].AdversaryPos.y = screenHeight;
}

void UpdateTrailPos(RBState &R){
    int RBF = R.CurrentFrame%60;
    R.S[RBF].Trail[0] = R.S[RBF].AdversaryPos;

    for(int i = TRAIL_S - 1; i > 0; i--){
        R.S[RBF].Trail[i] = R.S[RBF].Trail[i - 1];
    }
}

void UpdatePointsCount(RBState &R){
    int RBF = R.CurrentFrame%60;
    // Here TRAIL_S - 5 was chosen arbitrarily as the anchor to count points from.
    if(EuclideanDistance(R.S[RBF].PlayerPos, R.S[RBF].Trail[TRAIL_S - 5]) < POINTSRANGE){
        R.S[RBF].Points++;
        if(R.S[RBF].TextSize < TEXT_S_MAX) R.S[RBF].TextSize += 4;
        return;
    }
    if(R.S[RBF].TextSize > TEXT_S_MIN) R.S[RBF].TextSize -= 2;
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

void LogicLoop(RBState &R){
    R.CurrentFrame++;

    CopyLastState(R); 

    UpdatePlayerPos(R);
    UpdateAdversaryPos(R);
    UpdateTrailPos(R);

    UpdatePointsCount(R);

//    printf("Updated a frame!.\n");
//    fflush(stdout);
}
