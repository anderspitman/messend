#include <stdio.h>
#include <stdint.h>
#include "messend.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>


int main(int argc, char **argv) {

    messend_startup();

    MessendAcceptor acceptor = messend_acceptor_create(9001);

    MessendPeer peer = 0;
    
    while(!peer) {
        printf("loopy\n");
        peer = messend_acceptor_accept(acceptor);
        SDL_Delay(100);
    }

    MessendMessage message;
    message.data = (uint8_t*)"Hi from server";
    message.size = 14;
    messend_peer_send_message(peer, message);

    //MessendMessage* recvMessage = messend_peer_receive_message(peer);

    //for (int i = 0; i < recvMessage->size; i++) {
    //    printf("%c", ((uint8_t*)(recvMessage->data))[i]);
    //}
    //printf("\n");

    //messend_message_free(recvMessage);

    messend_peer_free(peer);
    peer = 0;

    messend_acceptor_free(acceptor);
    acceptor = NULL;

    messend_shutdown();

    return 0;
}
