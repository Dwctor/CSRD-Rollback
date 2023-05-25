# network-c utilization

This small library provides a simple API for implementing P2P communication.
The network state is managed through the **network** struct and its functions.
The messaging is done through the **MESSAGE** struct, an abstract interface 
that must be implemented by the calling code.

## How to connect and disconnect
In order to use the **network** functionality, a receiving and a sending port 
must be specified as integers. Network initialization is done through the 
**new_network** function. Connections are established when both hosts have 
called **network_start** and are terminated when both call **network_end**.
Example:
```
#import "network.c"
int main() {
    struct network nw = new_network(8080, 8081);
    network_start(&nw);
    // connection is available while both hosts run code in this interval
    network_end(&nw);
}
```
Please note that this code will not compile, for the **MESSAGE** interface was not 
implemented.

## The MESSAGE interface
**Messages are part of the network state**. Therefore, message structure, 
serialization and deserialization must be implemented if "network.c" is to be 
imported. This interface is comprised of:

- `struct MESSAGE`: Defines the message data type, and may contain whatever 
formatted data is deemed necessary.

- `char* SERIALIZE_MESSAGE(struct message* m)`: Data is passed through the 
network as an array of **char**s, thus it is necessary to define how this data is 
to be converted into such array. This function is called in **network_send**. 
Serialization must respect the network buffer size through the use of the 
**BUF_SIZE** constant defined in the "network.c" library.

- `void DESERIALIZE_MESSAGE(struct message* m, char* msg)`: Once data is 
received from the network, the **char** array must be restructured into the 
**MESSAGE** struct.
Example:
```
struct MESSAGE {
    int val;
    char* desc;
};

char* SERIALIZE_MESSAGE(struct MESSAGE* m) {
    char str[BUF_SIZE];
    // Escaping special characters
    if(m->desc[0] == MESSAGE_EXIT
       && m->desc[0] == MESSAGE_END) {
       strcat(str, "\"); 
    }
    strcat(str, m->desc);
    sprintf(str, "%s, value: %d, description: ", str, m->val);
    return str;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, char* msg) {
    // Removing escape characters
    if(msg[0] == '\') {
        msg += 1;
    }
    m->desc;
    m->val = atoi(msg);
}
```
### Escape characters
"network.c" uses the constants **MESSAGE_EXIT** and **MESSAGE_BEGIN** at the 
beginning of messages  to handle connection and messaging. Is is therefore 
**critical** that the serialization and deserialization functions take this 
into account.

## Sending and getting messages
Sending messages is done through the **network_send** function:
```
struct network nw = new_network(x, y);
// let's use the previously defined MESSAGE interface
struct MESSAGE* out_m;
m->val = 10;
network_start(&nw);
network_send(&nw, out_m);
```

Getting inbound messages is done through the **network_get** function. This 
function returns NULL if there are no messages since the last call, or a valid 
pointer if there is a new message.
```
struct network nw = new_network(x, y);
// let's use the previously defined MESSAGE interface
struct MESSAGE* in_m;
network_start(&nw);
in_m = network_get(&nw, m);
```

## Example

message.h:
```
struct MESSAGE {
    int val;
};

char* SERIALIZE_MESSAGE(struct MESSAGE* m) {
    char* str = malloc(sizeof(char)* BUF_SIZE);
    sprintf(str, "%d", m->val);
    return str;
}

void DESERIALIZE_MESSAGE(struct MESSAGE* m, char* msg) {
    m->val = atoi(msg);
}

```

server.c:
```
#include "network.c"
#include "message.h"

int main() {
    struct network nw = new_network(8080, 8081);
    struct MESSAGE* m = NULL;
    network_start(&nw);
    do {
        m = network_get(&nw);
    } while(m == NULL);
```
