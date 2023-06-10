#include <stdio.h>
#include <math.h>
#include "Game.h"

void VerifyPrediction(RBState &R, MESSAGE M){
  long ServerCurrentFrame = M.CurrentFrame;
  GameState S = M.S;

  int FDiff = R.CurrentFrame - ServerCurrentFrame; 
  if(FDiff > 60){
    printf("Delays got over one second, exiting...\n");
    fflush(stdout);
    exit(1);
  }
  if(FDiff < 0){
    printf("Delays got... negative? Exiting...\n");
    fflush(stdout);
    exit(1);
  }

  int RBF = R.CurrentFrame - FDiff;

  if(RBF < 0){
    RBF += RB_FRAMES;
  }

  if(HasPredictionFailed(R.S[RBF], S)){
    RollBack(R, S, FDiff);
  }

}

bool HasPredictionFailed(GameState a, GameState b){
  if(a.AdversaryInput.x != b.AdversaryInput.x || a.AdversaryInput.y != b.AdversaryInput.y){
    return true;
  }
  return false;
}

// Given that the prediction was wrong, does the rollback operation.
void RollBack(RBState &R, GameState S, int FDiff){ //FDiff is a positive, < RB_FRAMES number.
  int RBF = R.CurrentFrame - FDiff;

  if(RBF < 0){
    RBF += RB_FRAMES;
  }

  // Fixes one frame. 
  R.S[RBF].AdversaryInput.x = S.AdversaryInput.x;
  R.S[RBF].AdversaryInput.y = S.AdversaryInput.y;
  R.S[RBF].AdversaryPos.x = S.AdversaryPos.x;
  R.S[RBF].AdversaryPos.y = S.AdversaryPos.y;
  
  //Fixes the rest of the FDiff - 1 Frames.
  R.CurrentFrame = R.CurrentFrame - FDiff;
  for(int i = 0; i < FDiff - 1; i++) GameLoop(R);

}

void GameLoop(RBState &R){
    R.CurrentFrame++;

    CopyLastState(R); 
    UpdatePlayerInput(R);

    UpdatePlayerPos(R);
    UpdateAdversaryPos(R);
    UpdateTrailPos(R);

    UpdatePointsCount(R);


    printf("Updated a frame!.\n");
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
} 

void UpdateAdversaryPos(RBState &R){
  int RBF = R.CurrentFrame%60;
  R.S[RBF].AdversaryPos.x += R.S[RBF].AdversaryInput.x * PLR_SPD;
  R.S[RBF].AdversaryPos.y += R.S[RBF].AdversaryInput.y * PLR_SPD;
}

void UpdatePlayerInput(RBState &R){
    int RBF = R.CurrentFrame%60;
    R.S[RBF].PlayerInput.x = 0;
    R.S[RBF].PlayerInput.y = 0;
    if (IsKeyDown(KEY_RIGHT)) R.S[RBF].PlayerInput.x = 1;
    if (IsKeyDown(KEY_LEFT)) R.S[RBF].PlayerInput.x = -1;
    if (IsKeyDown(KEY_UP)) R.S[RBF].PlayerInput.y = -1;
    if (IsKeyDown(KEY_DOWN)) R.S[RBF].PlayerInput.y = +1;
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

