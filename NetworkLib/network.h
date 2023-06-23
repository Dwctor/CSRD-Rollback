#ifndef NETWORK_H
#define NETWORK_H
#include <sys/socket.h> // for inet_addr
#include <arpa/inet.h> // for sockets
#include <unistd.h> // just for the close() function
#include <cstdint>
#define BUF_SIZE 1024
#define MESSAGE_EXIT 'x'
#define MESSAGE_BEGIN 'b'
                   
// Reference: https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
struct socket_wrapper {
    int port;
    int desc;
    struct sockaddr_in socket;
};

struct msg_queue {
    uint8_t msg[BUF_SIZE];
    struct msg_queue* next;
};


struct network {
    pthread_t rec;
    pthread_t send;
    int connected;
    int accepted;
    struct socket_wrapper sender;
    struct socket_wrapper receiver;
    struct socket_wrapper client;
//    uint8_t last_msg[BUF_SIZE];
    struct msg_queue* msg_queue;
    struct msg_queue* msg_tail;
    struct network* self;
};
//
// MESSAGE types must be defined in the file that imports this library.
struct MESSAGE;
void SERIALIZE_MESSAGE(struct MESSAGE* m, uint8_t msg[BUF_SIZE]);
void DESERIALIZE_MESSAGE(struct MESSAGE* m, uint8_t msg[BUF_SIZE]);


struct network new_network(int rec_port, int send_port);
void network_start(struct network* nw);
void network_end(struct network* nw);
int network_send(struct network* nw, struct MESSAGE* m);
int network_get(struct network* nw, struct MESSAGE* m);

#endif
