/* easyvec - a simple vector implementation for C */
/* made by <franzageek> on the go */

#ifndef __EVEC_H__
#define __EVEC_H__
#define EVEC_VER 001

#include <intdef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct _evec
{
    u8* data;
    u8 capacity;
    u8 size;
    u8 item_size;
    bool valid;
} evec_t;

/*
void evec__stack_free(evec_t* evec)
{
    free(evec->data);
    memset(evec, 0, sizeof(evec_t));
    return;
}

evec_t evec__stack_new(u16 size_bytes)
{
    evec_t evec = {0};
    u8* data = calloc(8, size_bytes);
    if (!data)
    {
        perror("evec: calloc:");
        exit(EXIT_FAILURE);
    }

    evec.data = data;
    evec.capacity = 8;
    evec.size = 0;
    evec.item_size = size_bytes;
    evec.valid = true;
    data = NULL;
    return evec;
}
*/

void evec__free(evec_t* evec);

evec_t* evec__new(u16 size_bytes);

void evec__push(evec_t* evec, void* data);

void* evec__at(evec_t* evec, u16 index);

#endif
