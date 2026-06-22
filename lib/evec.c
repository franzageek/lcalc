#include <evec.h>

void evec__free(evec_t* evec)
{
    free(evec->data);
    memset(evec, 0, sizeof(evec_t));
    free(evec);
    return;
}

evec_t* evec__new(u16 size_bytes)
{
    evec_t* evec = calloc(1, sizeof(evec_t));
    if (!evec)
    {
        perror("evec: calloc:");
        exit(EXIT_FAILURE);
    }

    u8* data = calloc(8, size_bytes);
    if (!data)
    {
        perror("evec: calloc:");
        exit(EXIT_FAILURE);
    }

    evec->data = data;
    evec->capacity = 8;
    evec->size = 0;
    evec->item_size = size_bytes;
    evec->valid = true;
    data = NULL;
    return evec;
}

void _evec__push(evec_t* evec, void* data, char* _f, size_t _l)
{
    if (evec->valid)
    {
        if (evec->size >= evec->capacity)
        {
            u8* new_data = (u8*)calloc(evec->capacity + (evec->capacity / 2), evec->item_size);
            if (!new_data)
            {
                perror("evec: calloc:");
                evec__free(evec);
                exit(EXIT_FAILURE);
            }
            
            memmove(new_data, evec->data, evec->capacity * evec->item_size);
            if (memcmp(new_data, evec->data, evec->capacity * evec->item_size) != 0)
            {
                fprintf(stderr, "evec: an error occurred at \'%s:%zu\' after memmove() call: source and destination do not match\n", _f, _l);
                evec__free(evec);
                exit(EXIT_FAILURE);
            }
    
            free(evec->data);
            evec->data = new_data;
            new_data = NULL;
            evec->capacity += evec->capacity / 2;
        }

        u8* dest = &evec->data[evec->size * evec->item_size];
        memmove(dest, data, evec->item_size);
        if (memcmp(data, dest, evec->item_size) != 0)
        {
            fprintf(stderr, "evec: an error occurred at \'%s:%zu\' after memmove() call: source and destination do not match\n", _f, _l);
            evec__free(evec);
            exit(EXIT_FAILURE);
        }
        
        evec->size += 1;
        return;
    }
    else
    {
        fprintf(stderr, "evec: an error occurred after evec__push(): called on an invalid evec\n");
        exit(EXIT_FAILURE);
    }    
}

void* _evec__at(evec_t* evec, u16 index, char* _f, size_t _l)
{
    if (evec->valid)
    {
        if (index < evec->size)
            return (void*)&evec->data[index * evec->item_size];
        
        else
        {
            fprintf(stderr, "evec: an error occurred at \'%s:%zu\' after evec__at() call: attempt to read out of range (index: %u, size: %u)\n", _f, _l, index, evec->size);
            evec__free(evec);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "evec: an error occurred at \'%s:%zu\' after evec__at() call: called on an invalid evec\n", _f, _l);
        exit(EXIT_FAILURE);
    }
}