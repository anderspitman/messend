#include "messend.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {

    messend_startup();

    struct Peer* peer = messend_initiate("127.0.0.1", 9001);
    if (!peer) {
        return 1;
    }

    struct Message message;
    message.data = "Hi from client";
    message.size = 14;
    peer_send_message(peer, message);

    struct Message* recvMessage = peer_receive_message(peer);

    for (int i = 0; i < recvMessage->size; i++) {
        printf("%c", ((Uint8*)(recvMessage->data))[i]);
    }

    message_free(&message);
    message_free(recvMessage);

    peer_free(&peer);
    messend_shutdown();

    return 0;
}
