#include <stdio.h>
#include <stdint.h>
#include "messend.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
    SDLNet_Init();

    struct Peer peer = messend_accept(9001);

    struct Message message;
    message.data = "Hi from server";
    message.size = 14;
    peer_send_message(&peer, message);

    message = peer_receive_message(&peer);

    for (int i = 0; i < message.size; i++) {
        printf("%c", ((Uint8*)(message.data))[i]);
    }

    message_free(&message);

    peer_free(&peer);

    SDLNet_Quit();

    return 0;
}
