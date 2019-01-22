#include "messdl_net.h"
#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main(int argc, char **argv) {
    SDLNet_Init();

    struct Acceptor acceptor = acceptor_create();
    struct Peer peer = acceptor_accept(&acceptor, 9001);

    struct Message message;
    message.data = "Hi there";
    message.size = 8;
    peer_send_message(&peer, message);

    message = peer_receive_message(&peer);
    message_free(&message);

    peer_free(&peer);

    SDLNet_Quit();

    return 0;
}
