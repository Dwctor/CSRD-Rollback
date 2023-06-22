#ifndef BACKEND_H
#define BACKEND_H
#include "Common.h"
#include "message.h"

#define ADV_CHANGE_DIR 30

void VerifyPrediction(RBState &R, MESSAGE M);
bool HasPredictionFailed(GameState a, GameState b);
void RollBack(RBState &R, GameState S, int FDiff);
void ServerLoop(RBState &R);
void LogicLoop(RBState &R);
void UpdateAdversaryInput(RBState &R);

#endif // !BACKEND_H

