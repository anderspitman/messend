#ifndef __MESSEND_H__
#define __MESSEND_H__

#include <stdint.h>
#include <SDL2/SDL_net.h>

struct Acceptor {
};

struct Initiator {
};

struct Message {
    void* data;
    uint64_t size;
};

struct Peer {
    TCPsocket socket;
};

struct Acceptor acceptor_create();
struct Peer acceptor_accept(struct Acceptor* acceptor, int port);

struct Initiator initiator_create();
struct Peer initiator_initiate(struct Initiator* initiator, char* addr, int port);

void peer_send_message(struct Peer* peer, struct Message message);
struct Message peer_receive_message(struct Peer* peer);
void peer_free(struct Peer* peer);

void message_free(struct Message* message);

#endif //__MESSEND_H__
