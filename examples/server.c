#include <stdio.h>
#include <stdint.h>
#include "messend.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>


int main(int argc, char **argv) {

    messend_startup();

    MessendAcceptor acceptor = messend_acceptor_create(9001);

    printf("Waiting for connection\n");
    MessendPeer peer = messend_acceptor_accept_wait(acceptor);
    printf("Connection established\n");

    while (1) {

        MessendMessage* recvMessage = NULL;
        recvMessage = messend_peer_receive_message_wait(peer);

        printf("Message received:\n");

        for (int i = 0; i < recvMessage->size; i++) {
            printf("%c", ((uint8_t*)(recvMessage->data))[i]);
        }
        printf("\n");

        MessendMessage response;
        response.data = (uint8_t*)malloc(recvMessage->size);
        response.size = recvMessage->size;
        for (int i = 0; i < response.size; i++) {
            response.data[i] = recvMessage->data[i];
        }

        messend_peer_send_message(peer, response);
        messend_message_free(recvMessage);
    }

    messend_peer_free(peer);
    peer = 0;

    messend_acceptor_free(acceptor);
    acceptor = NULL;

    messend_shutdown();

    return 0;
}
