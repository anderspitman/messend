#ifndef __MESSEND_H__
#define __MESSEND_H__

#include <stdint.h>
#include <SDL2/SDL_net.h>


typedef struct _Acceptor* Acceptor;
typedef struct _Peer* Peer;

struct Message {
    void* data;
    uint64_t size;
};

void messend_startup();
void messend_shutdown();

Acceptor acceptor_create(uint16_t port);
Peer acceptor_accept(Acceptor acceptor);
void acceptor_free(Acceptor acceptor);

Peer messend_initiate(char* addr, int port);

void peer_send_message(Peer peer, struct Message message);
struct Message* peer_receive_message(Peer peer);
void peer_free(Peer peer);

void message_free(struct Message* message);

#endif //__MESSEND_H__
