#ifndef GAME_H
#define GAME_H
#include "Common.h"
#include <raylib.h>

void VerifyPrediction(RBState &R, MESSAGE M);

bool HasPredictionFailed(GameState a, GameState b);

void RollBack(RBState &R, GameState S, int FDiff);

//Updates the Player's position given input on the arrow keys.
void UpdatePlayerInput(RBState &);

//Runs the main game loop (the state update part), which includes updating all positions.
void GameLoop(RBState &);

#endif // !GAMEH
