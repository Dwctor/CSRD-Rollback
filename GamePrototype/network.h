#ifndef NETWORK_C
#define NETWORK_C
#include "../NetworkLib/network.cpp"

struct MESSAGE {
    int val;
};

char* SERIALIZE_MESSAGE(struct MESSAGE* m) {
    char* str = (char*)malloc(sizeof(char)* BUF_SIZE);
    sprintf(str, "%d\n", m->val);
    return str;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, char* msg) {
    m->val = atoi(msg);
}
#endif
