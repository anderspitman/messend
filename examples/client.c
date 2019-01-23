#include "messend.h"


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
        printf("%c", ((uint8_t*)(recvMessage->data))[i]);
    }
    printf("\n");

    message_free(recvMessage);
    recvMessage = 0;

    peer_free(peer);
    peer = 0;
    messend_shutdown();

    return 0;
}
