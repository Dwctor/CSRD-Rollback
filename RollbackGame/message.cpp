#include <raylib.h>
#include <string>
#include <stdio.h>
#include "Common.h"
#include <string.h>
#include "../NetworkLib/network.h"
#include "message.h"

/* Messages are serialized as a one-line concatenation of:
 * (4 bytes)        playerpos
 * (4 bytes)        adversarypos
 * (TAIL_S*4 bytes) trail
 * (8 bytes)        points
 * (4 bytes)        playerinput
 * (4 bytes)        adversaryinput
 * (2 bytes)        textsize
 * (8 bytes)        currentframe
 *
 * Since all data types and sizes are know, no separator will be used, and only
 * the position in the array will be considered. This is simpler than escaping
 * special characters.
 * Special attention must be given to the constants MESSAGE_BEGIN and 
 * MESSAGE_EXIT, which must not occur in the first character of the message. 
 * This is acheived by inserting a guard initial character.
 */

void serialize_vector2(uint8_t str[BUF_SIZE], Vector2 v, int pos) {
    float x = v.x;
    float y = v.y;
    int sf = sizeof(float);
    memcpy(str+pos, &x, sf);
    memcpy(str+pos+sf, &y, sf);
}

void serialize_vector2_arr(uint8_t str[BUF_SIZE],
                           Vector2 v[TRAIL_S],
                           int pos) {
    int sf = sizeof(float);
    float x, y;
    for (int i = 0; i < TRAIL_S; ++i) {
        x = v[i].x;
        y = v[i].y;
        memcpy(str+pos+i*sf, &x, sf);
        memcpy(str+pos+sf+i*sf, &y, sf);
    }
}

void serialize_long(uint8_t str[BUF_SIZE], long l, int pos) {
    memcpy(str+pos, &l, sizeof(long));
}

void serialize_int(uint8_t str[BUF_SIZE], int i, int pos) {
    memcpy(str+pos, &i, sizeof(int));
}

void SERIALIZE_MESSAGE(struct MESSAGE* m, uint8_t str[BUF_SIZE]) {
    fprintf(stderr, "Sending adversary pos: %.2f, %.2f\n",
            m->S.AdversaryPos.x, m->S.AdversaryPos.y);
    fprintf(stderr, "Sending player pos: %.2f, %.2f\n",
            m->S.PlayerPos.x, m->S.PlayerPos.y);
//	fprintf(stderr, "Serializing message...");
    char safeguard = MESSAGE_BEGIN ^ MESSAGE_EXIT;
    int pos = 1;
    int vector_size = 2*sizeof(float);
    int long_size = sizeof(long);
    int int_size = sizeof(int); str[0] = safeguard;
//	fprintf(stderr, "Serialized safeguard...");
    serialize_vector2(str, m->S.PlayerPos, pos);
    pos += vector_size;
//	fprintf(stderr, "Serialized playerpos...");
    serialize_vector2(str, m->S.AdversaryPos, pos);
    pos += vector_size;
//	fprintf(stderr, "Serialized adversarypos...");
    serialize_vector2_arr(str, m->S.Trail, pos);
    pos += TRAIL_S*vector_size;
//	fprintf(stderr, "Serialized trail...");
    serialize_long(str, m->S.Points, pos);
    pos += long_size;
//	fprintf(stderr, "Serialized points...");
    serialize_vector2(str, m->S.PlayerInput, pos);
    pos += vector_size;
//	fprintf(stderr, "Serialized playerinput...");
    serialize_vector2(str, m->S.AdversaryInput, pos);
    pos += vector_size;
//	fprintf(stderr, "Serialized adversaryinput...");
    serialize_int(str, m->S.TextSize, pos);
    pos += int_size;
//	fprintf(stderr, "Serialized textsize...");
    serialize_long(str, m->CurrentFrame, pos);
    pos += long_size;
//	fprintf(stderr, "Serialized currentframe...");
}

Vector2 deserialize_vector2(uint8_t msg[BUF_SIZE], int pos) {
    float x,y;
    Vector2 vec;
    int sf = sizeof(float);
    memcpy(&x, msg+pos, sf);
    memcpy(&y, msg+pos+sf, sf);
    vec = {x, y};
    return vec;
}

long deserialize_long(uint8_t msg[BUF_SIZE], int pos) {
    long x;
    memcpy(&x, msg+pos, sizeof(x));
    return x;
}

int deserialize_int(uint8_t msg[BUF_SIZE], int pos) {
    int x;
    memcpy(&x, msg+pos, sizeof(x));
    return x;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, uint8_t msg[BUF_SIZE]) {
    int pos = 1;
    int vector2_step = 2*sizeof(float);
    int long_step = sizeof(long);
    int int_step = sizeof(int);
    // PLayerPos
    m->S.PlayerPos = deserialize_vector2(msg, pos);
    pos += vector2_step;
    // AdversaryPos
    m->S.AdversaryPos = deserialize_vector2(msg, pos);
    pos += vector2_step;
    // Trail[TRAIL_S]
    for (int i=0; i<TRAIL_S; ++i) {
        m->S.Trail[i] = deserialize_vector2(msg, pos);
        pos += vector2_step;
    }
    // Points
    m->S.Points = deserialize_long(msg, pos);
    pos += long_step;
    // PlayerInput
    m->S.PlayerInput = deserialize_vector2(msg, pos);
    pos += vector2_step;
    // AdversaryInput
    m->S.AdversaryInput = deserialize_vector2(msg, pos);
    pos += vector2_step;
    // TextSize
    m->S.TextSize = deserialize_int(msg, pos);
    pos += int_step;
    // CurrentFrame
    m->CurrentFrame = deserialize_long(msg, pos);
}
