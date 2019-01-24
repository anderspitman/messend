#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "messend.h"

struct _Acceptor {
    TCPsocket socket;
};

struct _Peer {
    SDLNet_SocketSet set;
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

MessendPeer peer_create(TCPsocket socket) {
    MessendPeer peer = (MessendPeer)malloc(sizeof(*peer));

    peer->set = SDLNet_AllocSocketSet(1);
    if (!peer->set) {
        error("allocating socket set");
    }

    SDLNet_TCP_AddSocket(peer->set, socket);

    peer->socket = socket;
    return peer;
}

MessendAcceptor messend_acceptor_create(const char* host, uint16_t port) {

    IPaddress ip;

    MessendAcceptor acceptor;
    acceptor = (MessendAcceptor)malloc(sizeof(*acceptor));

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

MessendPeer messend_acceptor_accept(MessendAcceptor acceptor) {

    MessendPeer peer = NULL;

    TCPsocket client = SDLNet_TCP_Accept(acceptor->socket);

    if (client) {
        peer = peer_create(client);
    }

    return peer;
}

MessendPeer messend_acceptor_accept_wait(MessendAcceptor acceptor) {
    MessendPeer peer = NULL;

    while (!peer) {
        peer = messend_acceptor_accept(acceptor);
        SDL_Delay(32);
    }

    return peer;
}

void messend_acceptor_free(MessendAcceptor acceptor) {
    SDLNet_TCP_Close(acceptor->socket);
    free(acceptor);
}

MessendPeer messend_initiate(char* addr, int port) {
    IPaddress ip;

    MessendPeer peer = 0;

    if (SDLNet_ResolveHost(&ip, addr, port)) {
        error("could not resolve host");
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        error("could not open socket");
    }

    peer = peer_create(socket);

    return peer;
}


bool messend_peer_has_message(MessendPeer peer) {
    SDLNet_CheckSockets(peer->set, 0);
    return SDLNet_SocketReady(peer->socket);
}

void messend_peer_send_message(MessendPeer peer, MessendMessage message) {
    Uint8 size_buf[sizeof(Uint32)];
    _SDLNet_Write32(message.size, size_buf);
    //printf("send size: %lu\n", message.size);

    SDLNet_TCP_Send(peer->socket, size_buf, sizeof(Uint32));
    SDLNet_TCP_Send(peer->socket, message.data, message.size);
}

MessendMessage* messend_peer_receive_message(MessendPeer peer) {

    // poll to see if there's data
    SDLNet_CheckSockets(peer->set, 0);

    if (!SDLNet_SocketReady(peer->socket)) {
        return NULL;
    }
    else {
        MessendMessage message = messend_peer_receive_message_wait(peer);
        MessendMessage* pmessage = (MessendMessage*)malloc(sizeof(MessendMessage));
        pmessage->data = message.data;
        pmessage->size = message.size;
        return pmessage;
    }
}

MessendMessage messend_peer_receive_message_wait(MessendPeer peer) {
    Uint8 size_buf[sizeof(Uint32)];

    if (SDLNet_TCP_Recv(peer->socket, size_buf, sizeof(Uint32)) <= 0) {
        // TODO: handle disconnect
        error("could not read size");
    }

    uint64_t size = _SDLNet_Read32(size_buf);

    Uint8* data_buf = (Uint8*)malloc(size);

    if (SDLNet_TCP_Recv(peer->socket, data_buf, size) <= 0) {
        error("failed to receive packet");
    }

    MessendMessage message;
    message.data = data_buf;
    message.size = size;

    return message;

}

void messend_peer_free(MessendPeer peer) {
    SDLNet_FreeSocketSet(peer->set);
    SDLNet_TCP_Close(peer->socket);
    free(peer);
}

void messend_message_free(MessendMessage message) {
    if (message.data) {
        free(message.data);
        message.data = 0;
    }
}
