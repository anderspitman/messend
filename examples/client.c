#include <stdio.h>
#include <stdint.h>
#include "messend.h"


int main(int argc, char **argv) {

    messend_startup();

    Peer peer = messend_initiate("127.0.0.1", 9001);
    if (!peer) {
        return 1;
    }

    Message message;
    //message.data = (uint8_t*)"Hi from client aaaaaaaaa";
    //message.size = 24;

    const uint64_t SIZE = 128;
    message.data = (uint8_t*)malloc(SIZE);
    for (int i = 0; i < SIZE; i += 4) {
        message.data[i] = 'A';
        message.data[i+1] = 'C';
        message.data[i+2] = 'G';
        message.data[i+3] = 'T';
    }
    message.size = SIZE;
    
    peer_send_message(peer, message);

    Message* recvMessage = peer_receive_message(peer);

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
