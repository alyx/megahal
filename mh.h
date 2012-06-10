#pragma once
#ifndef MH_H
#define MH_H

#include <libmowgli-2/mowgli.h>

/* typedef these structs into our publicly usable
 * data structures
 */
typedef struct mh_string_ MegahalString;
typedef struct mh_dict_   MegahalDict;
typedef struct mh_swap_   MegahalSwap;
typedef struct mh_node_   MegahalTree;
typedef struct mh_model_  MegahalModel;
typedef struct mh_state_  Megahal;

struct mh_state_
{
    void (*err)(const char * title, char * fmt, ...);
    void (*warn)(char * title, char * fmt, ...);
    void (*status)(char * fmt, ...);
    char *dir;
    mowgli_patricia_t * words;
};

struct mh_string_
{
    size_t len;
    char * word;
};

struct mh_dict_
{
    unsigned long size;
    MegahalString ** entry;
    unsigned short * index;
};

struct mh_swap_
{
    unsigned short size;
    MegahalString * from, * to;
};

struct mh_node_
{
    unsigned short symbol;
    unsigned long usage;
    unsigned short count;
    unsigned short branch;
    struct mh_node_ ** tree;
};

struct mh_model_
{
    unsigned char order;
    MegahalTree * next, * prev, ** ctx;
    MegahalDict * dict;
};

void mh_init(Megahal *state, void (*err)(const char * title, char * fmt, ...),
        void (*warn)(char * title, char * fmt, ...), 
        void (*status)(char * fmt, ...));


#endif /* MH_H */
