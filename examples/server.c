#include <stdio.h>
#include <stdint.h>
#include "messend.h"


int main(int argc, char **argv) {

    messend_startup();

    struct Peer* peer = 0;
    
    while(!peer) {
        peer = messend_accept(9001);
    }

    struct Message message;
    message.data = "Hi from server";
    message.size = 14;
    peer_send_message(peer, message);

    struct Message* recvMessage = peer_receive_message(peer);

    for (int i = 0; i < recvMessage->size; i++) {
        printf("%c", ((uint8_t*)(recvMessage->data))[i]);
    }
    printf("\n");

    message_free(&message);
    message_free(recvMessage);

    peer_free(peer);
    peer = 0;

    messend_shutdown();

    return 0;
}
