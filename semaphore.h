//
// Created by corentin on 03/05/15.
//

#ifndef STREAMS_SEMAPHORE_H
#define STREAMS_SEMAPHORE_H

#define NB_SEMAPHORES 2
#define SINGLE_OPERATION 1

#define SEMAPHORE_MUTEX 0
#define SEMAPHORE_ACCESS 1

void init_semaphore();
int perform_semaphore_control(int semaphore, int cmd);
int semaphore_up(int semaphore);
int semaphore_down(int semaphore);

#endif //STREAMS_SEMAPHORE_H
