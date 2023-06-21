#include <stdio.h>
#include <math.h>
#include "Game.h"
#include "Common.h"

void VerifyPrediction(RBState &R, MESSAGE M){
  long ServerCurrentFrame = M.CurrentFrame;
  GameState S = M.S;

  int FDiff = R.CurrentFrame - ServerCurrentFrame; 
  fprintf(stderr, "Got FDiff: %d\n", FDiff);
  
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
  RBF %= 60;

  if(HasPredictionFailed(R.S[RBF], S)){
    RollBack(R, S, FDiff);
  }

}

bool HasPredictionFailed(GameState a, GameState b){
  if(a.AdversaryInput.x != b.AdversaryInput.x || a.AdversaryInput.y != b.AdversaryInput.y){
    fprintf(stderr, "Got predfailed\n");
    return true;
  }
  fprintf(stderr, "Got success\n");
  return false;
}

// Given that the prediction was wrong, does the rollback operation.
void RollBack(RBState &R, GameState S, int FDiff){ //FDiff is a positive, < RB_FRAMES number.
  int RBF = R.CurrentFrame - FDiff;

  if(RBF < 0){
    RBF += RB_FRAMES;
  }
  RBF %= 60;

  // Fixes one frame. 
  R.S[RBF].AdversaryInput.x = S.AdversaryInput.x;
  R.S[RBF].AdversaryInput.y = S.AdversaryInput.y;
  R.S[RBF].AdversaryPos.x = S.AdversaryPos.x;
  R.S[RBF].AdversaryPos.y = S.AdversaryPos.y;
  
  //Fixes the rest of the FDiff - 1 Frames.
  fprintf(stderr, "R.CurrentFrame before: %d\n", R.CurrentFrame);
  fprintf(stderr, "PlayerPos before: %4f %4f\n", R.S[R.CurrentFrame%60].PlayerPos.x, R.S[R.CurrentFrame%60].PlayerPos.y);
  R.CurrentFrame = R.CurrentFrame - FDiff;
  fprintf(stderr, "R.CurrentFrame during: %d\n", R.CurrentFrame);
  for(int i = 0; i < FDiff; i++) LogicLoop(R);
  fprintf(stderr, "PlayerPos after: %4f %4f\n", R.S[R.CurrentFrame%60].PlayerPos.x, R.S[R.CurrentFrame%60].PlayerPos.y);
  fprintf(stderr, "R.CurrentFrame after: %d\n", R.CurrentFrame);

}

void GameLoop(RBState &R){
    R.CurrentFrame++;

    CopyLastState(R); 
    UpdatePlayerInput(R);

    UpdatePlayerPos(R);
    UpdateAdversaryPos(R);
    UpdateTrailPos(R);

    UpdatePointsCount(R);


//    printf("Updated a frame!.\n");
//    fflush(stdout);
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

