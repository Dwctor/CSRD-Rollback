#include <stdio.h>
#include "network.cpp"

struct MESSAGE {
    int val;
};

uint8_t* SERIALIZE_MESSAGE(struct MESSAGE* m) {
    char* str = (char*)malloc(sizeof(char)* BUF_SIZE);
    sprintf(str, "%d", m->val);
    return str;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, uint8_t* msg) {
    m->val = atoi((char*)msg);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("srv call must include two port numbers\n");
        return -1;
    }

    int rec_port = atoi(argv[1]);
    int send_port = atoi(argv[2]);
    struct network nw = new_network(rec_port, send_port);
    struct MESSAGE m;
    m.val = 32;
    network_start(&nw);
    network_send(&nw, &m);
    network_end(&nw);
    return 0;
}
