#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL_net.h>
#include "messend.h"

struct _Acceptor {
    TCPsocket socket;
};

struct _Peer {
    TCPsocket socket;
};

void error(const char* message) {
    fprintf(stderr, "ERROR: %s\n", message);
    exit(1);
}


void messend_startup() {
    SDLNet_Init();
}

void messend_shutdown() {
    SDLNet_Quit();
}

Acceptor acceptor_create(uint16_t port) {

    IPaddress ip;

    Acceptor acceptor;
    acceptor = malloc(sizeof(*acceptor));

    if (SDLNet_ResolveHost(&ip, NULL, port)) {
        error(SDLNet_GetError());
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        error(SDLNet_GetError());
    }

    acceptor->socket = socket;

    return acceptor;
}

Peer acceptor_accept(Acceptor acceptor) {

    Peer peer = NULL;

    TCPsocket client = SDLNet_TCP_Accept(acceptor->socket);

    if (client) {
        peer = malloc(sizeof(*peer));
        peer->socket = client;
    }

    return peer;
}

void acceptor_free(Acceptor acceptor) {
    SDLNet_TCP_Close(acceptor->socket);
    free(acceptor);
}

Peer messend_initiate(char* addr, int port) {
    IPaddress ip;

    Peer peer = 0;

    if (SDLNet_ResolveHost(&ip, addr, port)) {
        error("could not resolve host");
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        error("could not open socket");
    }

    peer = malloc(sizeof(*peer));
    peer->socket = socket;

    return peer;
}


void peer_send_message(Peer peer, Message message) {
    Uint8 size_buf[sizeof(Uint32)];
    //size_buf[0] = message.size;
    _SDLNet_Write32(message.size, size_buf);

    SDLNet_TCP_Send(peer->socket, size_buf, sizeof(Uint32));
    SDLNet_TCP_Send(peer->socket, message.data, message.size);
}

Message* peer_receive_message(Peer peer) {

    Uint8 size_buf[sizeof(Uint32)];

    if (SDLNet_TCP_Recv(peer->socket, size_buf, sizeof(Uint32)) <= 0) {
        error("could not read size");
    }

    uint64_t size = _SDLNet_Read32(size_buf);

    Uint8* data_buf = malloc(size);

    if (SDLNet_TCP_Recv(peer->socket, data_buf, size) <= 0) {
        error("failed to receive packet");
    }

    Message* message = malloc(sizeof(Message));
    message->data = data_buf;
    message->size = size;

    return message;
}

void peer_free(Peer peer) {
    SDLNet_TCP_Close(peer->socket);
    free(peer);
}

void message_free(Message* message) {
    if (message->data) {
        free(message->data);
        message->data = 0;
    }
}
