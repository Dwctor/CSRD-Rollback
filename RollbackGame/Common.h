#ifndef COMMON
#define COMMON

#include <raylib.h>
const int screenWidth = 1200;
const int screenHeight = 720;

#define FPS 60       // The Frames Per Second of the game.
#define RB_FRAMES 60 // The ammount of frames the rollback can go back to.
#define COUNTER_MESSAGE 1 // The ammount of frames to send and receive rollback information.

// These defines quickly tweak the behaviour of the game. In this short example we keep them here, but in the final project they should be moved to their own header.
#define TRAIL_S 50
#define TRAIL_R 1
#define ADV_SPD 10
#define PLR_SPD 8
#define POINTSRANGE 55
#define TEXT_S_MAX 80
#define TEXT_S_MIN 40

typedef struct GameState {

    Vector2 PlayerPos;
    Vector2 AdversaryPos;
    Vector2 Trail [TRAIL_S];
    
    long Points;

    Vector2 PlayerInput; // Represents the player input, where 0 for no movement and 1 and -1 for movement in that direction.
    Vector2 AdversaryInput; // Same, but for the adversary.
    
    // Is client-sided (doesn't go to network)
    int TextSize;

} GameState;

typedef struct RBState{
    GameState S[RB_FRAMES]; 
    long CurrentFrame;
}RBState;

void InitGame(RBState &);

void InitState(RBState &);

void CopyLastState(RBState &);

void UpdatePlayerPos(RBState &); 

void UpdateAdversaryPos(RBState &);

//Makes the trail "trail" behind the Adversary node.
void UpdateTrailPos(RBState &);

//Updates the points of the player. Incremented when the player is touching the tail of the trail of the adversary.
void UpdatePointsCount(RBState &);

//Will draw a single frame of the game.
void DrawState(GameState &);

//Returns the euclidean distance of two Vector2 points.
double EuclideanDistance(Vector2 a, Vector2 b);

//Does the game/server loop without new inputs being provided.
void LogicLoop(RBState &);
#endif // !COMMON
