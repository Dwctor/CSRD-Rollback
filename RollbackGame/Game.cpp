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

