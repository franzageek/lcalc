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
    u16 capacity;
    u16 size;
    u16 item_size;
    bool valid;
} evec_t;

void evec__free(evec_t* evec);

evec_t* evec__new(u16 size_bytes);

void _evec__push(evec_t* evec, void* data, char* _f, size_t _l);

#ifndef evec__push
#define evec__push(ptr_to_evec, ptr_to_data) _evec__push(ptr_to_evec, ptr_to_data, __FILE__, __LINE__)
#endif

void* _evec__at(evec_t* evec, u16 index, char* _f, size_t _l);

#ifndef evec__at
#define evec__at(ptr_to_evec, u16_index) _evec__at(ptr_to_evec, u16_index, __FILE__, __LINE__)
#endif

#endif
