#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "messdl_net.h"

void error(const char* message) {
    fprintf(stderr, "ERROR: %s\n", message);
    exit(1);
}

static int acceptor_count = 0;

struct Acceptor acceptor_create() {
    acceptor_count++;

    if (acceptor_count == 1) {
    }

    struct Acceptor acceptor;
    return acceptor;
}


struct Peer acceptor_accept(struct Acceptor* acceptor, int port) {
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, NULL, port)) {
        error(SDLNet_GetError());
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        error(SDLNet_GetError());
    }

    struct Peer peer;

    int done = 0;
    while (!done) {

        TCPsocket client = SDLNet_TCP_Accept(socket);

        if (!client) {
            //error(SDLNet_GetError());
            SDL_Delay(100);
        }
        else {
            peer.socket = client;
            done = 1;
        }
    }

    return peer;
}

struct Initiator initiator_create() {
    struct Initiator initiator;
    return initiator;
}

struct Peer initiator_initiate(struct Initiator* initiator, char* addr, int port) {
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, addr, port)) {
        error(SDLNet_GetError());
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        error(SDLNet_GetError());
    }

    struct Peer peer;

    peer.socket = socket;
    return peer;
}


void peer_send_message(struct Peer* peer, struct Message message) {
    Uint8 size_buf[1];
    size_buf[0] = message.size;

    SDLNet_TCP_Send(peer->socket, size_buf, 1);
    SDLNet_TCP_Send(peer->socket, message.data, message.size);
}

struct Message peer_receive_message(struct Peer* peer) {
    Uint8 size_buf[1];


    if (SDLNet_TCP_Recv(peer->socket, size_buf, 1) <= 0) {
        error(SDLNet_GetError());
    }

    uint64_t size = size_buf[0];
    printf("size: %lu\n", size);

    Uint8* data_buf = malloc(size);

    if (SDLNet_TCP_Recv(peer->socket, data_buf, size) <= 0) {
        error(SDLNet_GetError());
    }

    struct Message message;
    message.data = data_buf;
    message.size = size;

    return message;
}

void peer_free(struct Peer* peer) {
    SDLNet_TCP_Close(peer->socket);
}

void message_free(struct Message* message) {
    if (message->data) {
        free(message->data);
        message->data = 0;
    }
}


