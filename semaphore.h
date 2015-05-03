//
// Created by corentin on 03/05/15.
//

#ifndef STREAMS_SEMAPHORE_H
#define STREAMS_SEMAPHORE_H

#define SINGLE_SEMAPHORE 1
#define FIRST_SEMAPHORE 0

void init_semaphore();
int perform_semaphore_control(int cmd);
int semaphore_up();
int semaphore_down();

#endif //STREAMS_SEMAPHORE_H
