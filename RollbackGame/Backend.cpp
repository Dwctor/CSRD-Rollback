#include <stdio.h>
#include <stdlib.h>
#include "Backend.h"
#include "Common.h"

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
  if(a.PlayerInput.x != b.PlayerInput.x || a.PlayerInput.y != b.PlayerInput.y){
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
  R.S[RBF].PlayerInput.x = S.PlayerInput.x;
  R.S[RBF].PlayerInput.y = S.PlayerInput.y;
  R.S[RBF].PlayerPos.x = S.PlayerPos.x;
  R.S[RBF].PlayerPos.y = S.PlayerPos.y;
  R.S[RBF].Points = S.Points;
  
  //Fixes the rest of the FDiff - 1 Frames.
  R.CurrentFrame = R.CurrentFrame - FDiff;
  for(int i = 0; i < FDiff - 1; i++) ServerLoop(R);

}

void ServerLoop(RBState &R){
    R.CurrentFrame++;

    CopyLastState(R); 

    UpdateAdversaryInput(R);

    UpdatePlayerPos(R);
    UpdateAdversaryPos(R);
    UpdateTrailPos(R);

    UpdatePointsCount(R);


//    printf("Updated a frame!.\n");
//    fflush(stdout);
}

//Every ADV_CHANGE_DIR Frames randomly chooses a direction to go.
void UpdateAdversaryInput(RBState &R){
  int RBF = R.CurrentFrame % 60;
  if(R.CurrentFrame % ADV_CHANGE_DIR == 0){
    R.S[RBF].AdversaryInput.x = rand()%3 - 1;
    R.S[RBF].AdversaryInput.y = rand()%3 - 1;
  }
}
