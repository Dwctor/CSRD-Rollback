#include <stdio.h>
#include "network.c"

struct MESSAGE {
    int val;
};

char* SERIALIZE_MESSAGE(struct MESSAGE* m) {
    char* str = malloc(sizeof(char)* BUF_SIZE);
    sprintf(str, "%d\n", m->val);
    return str;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, char* msg) {
    m->val = atoi(msg);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("srv call must include two port numbers\n");
        return -1;
    }

    int rec_port = atoi(argv[1]);
    int send_port = atoi(argv[2]);
    struct network nw = new_network(rec_port, send_port);
    struct MESSAGE* m = NULL;
    network_start(&nw);
    do {
        m = network_get(&nw);
    } while(m == NULL);
    printf("message after deserialization: %d\n", m->val);
    network_end(&nw);
    return 0;
}
