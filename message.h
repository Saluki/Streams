//
// Created by coco on 26/04/15.
//

#ifndef STREAMS_MESSAGE_H
#define STREAMS_MESSAGE_H

#define MESSAGE_LENGTH 25

enum message_type_t { CANCEL_GAME, REGISTRATE, VALID_REGISTRATION, NEW_PICK, CHOOSE_PICK, END_GAME };

char* encode(enum message_type_t type, void* payload);
void* decode(char* message);

#endif //STREAMS_MESSAGE_H