#ifndef GAME_H
#define GAME_H
#include "Common.h"
#include <raylib.h>

void VerifyPrediction(RBState &R, MESSAGE M);

bool HasPredictionFailed(GameState a, GameState b);

void RollBack(RBState &R, GameState S, int FDiff);

//Updates the Player's position given input on the arrow keys.
void UpdatePlayerInput(RBState &);

//Makes the trail "trail" behind the Adversary node.
void UpdateTrailPos(RBState &);

//Updates the points of the player. Incremented when the player is touching the tail of the trail of the adversary.
void UpdatePointsCount(RBState &);

//Will draw a single frame of the game.
void DrawState(GameState &);

//Runs the main game loop (the state update part), which includes updating all positions.
void GameLoop(RBState &);

void CopyLastState(RBState &);

void UpdatePlayerPos(RBState &); 

void UpdateAdversaryPos(RBState &);

#endif // !GAMEH
