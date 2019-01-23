#ifndef __MESSEND_H__
#define __MESSEND_H__

#include <stdint.h>
#include <SDL2/SDL_net.h>


typedef struct _Acceptor* Acceptor;

struct Message {
    void* data;
    uint64_t size;
};

struct Peer {
    TCPsocket socket;
};


void messend_startup();
void messend_shutdown();

Acceptor acceptor_create(uint16_t port);
struct Peer* acceptor_accept(Acceptor acceptor);
void acceptor_free(Acceptor acceptor);

struct Peer* messend_initiate(char* addr, int port);

void peer_send_message(struct Peer* peer, struct Message message);
struct Message* peer_receive_message(struct Peer* peer);
void peer_free(struct Peer* peer);

void message_free(struct Message* message);

#endif //__MESSEND_H__
