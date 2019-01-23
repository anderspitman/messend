#include <stdio.h>
#include <stdint.h>
#include "messend.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>


int main(int argc, char **argv) {

    messend_startup();

    Acceptor acceptor = acceptor_create(9001);

    Peer peer = 0;
    
    while(!peer) {
        printf("loopy\n");
        peer = acceptor_accept(acceptor);
        SDL_Delay(100);
    }

    Message message;
    message.data = "Hi from server";
    message.size = 14;
    peer_send_message(peer, message);

    Message* recvMessage = peer_receive_message(peer);

    for (int i = 0; i < recvMessage->size; i++) {
        printf("%c", ((uint8_t*)(recvMessage->data))[i]);
    }
    printf("\n");

    message_free(recvMessage);

    peer_free(peer);
    peer = 0;

    acceptor_free(acceptor);
    acceptor = NULL;

    messend_shutdown();

    return 0;
}
