#include "messdl_net.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
    SDLNet_Init();

    struct Initiator initiator = initiator_create();
    struct Peer peer = initiator_initiate(&initiator, "127.0.0.1", 9001);

    struct Message message;
    message.data = "Hi from client";
    message.size = 14;
    peer_send_message(&peer, message);

    message = peer_receive_message(&peer);
    for (int i = 0; i < message.size; i++) {
        printf("%c", ((Uint8*)(message.data))[i]);
    }

    SDLNet_Quit();
    return 0;
}
