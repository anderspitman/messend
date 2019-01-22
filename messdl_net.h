#ifndef __MESSDL_NET__
#define __MESSDL_NET__

#include <stdint.h>
#include <SDL2/SDL_net.h>

struct Acceptor {
};

struct Message {
    void* data;
    uint64_t size;
};

struct Peer {
    TCPsocket socket;
};

struct Acceptor acceptor_create();
void peer_send_message(struct Peer* peer, struct Message message);
struct Message peer_receive_message(struct Peer* peer);
void peer_free(struct Peer* peer);
void message_free(struct Message* message);
struct Peer acceptor_accept(struct Acceptor* acceptor, int port);

#endif //__MESSDL_NET__
