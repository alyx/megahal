#include <libmowgli-2/mowgli.h>

/*#include "megahal.h"*/

typedef struct mh_string_ MegahalString;
typedef struct mh_dict_   MegahalDict;
typedef struct mh_swap_   MegahalSwap;
typedef struct mh_node_   MegahalTree;
typedef struct mh_model_  MegahalModel;

/* mh.h */
typedef struct mh_state_ Megahal;

struct mh_state_
{
    void (*err)(const char * title, char * fmt, ...);
    void (*warn)(char * title, char * fmt, ...);
    void (*status)(char * fmt, ...);
    char *dir;
    MegahalDict * words, * greets;
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


unsigned short mh_word_add(Megahal *state, MegahalDict * dict, MegahalString * word);

MegahalDict * mh_dictionary_new(Megahal *state)
{
    MegahalDict * dict;

    dict = mowgli_alloc(sizeof(MegahalDict));

    if (!dict)
    {
        state->err("mh_dictionary_new", "Dictionary allocation failed.");
        return NULL;
    }

    dict->size = 0;
    dict->index = NULL;
    dict->entry = NULL;

    return dict;
}

void mh_dictionary_save(Megahal *state, FILE * fp, MegahalDict * dict)
{
    unsigned int i;
    int ret;

    fwrite(&dict->size, sizeof(unsigned long), 1, fp);
    if (ferror(fp))
    {
        state->err("mh_dictionary_save", strerror(errno));
    }

    state->status("Saving dictionary");
    for (i = 0; i < dict->size; ++i)
    {
        mh_word_save(state, fp, dictionary->entry[i]);
    }
}

void mh_init(Megahal *state, void (*err)(const char * title, char * fmt, ...),
        void (*warn)(char * title, char * fmt, ...), 
        void (*status)(char * fmt, ...))
{
    state->err = err;
    state->warn = warn;
    state->status = status;

    state->words  = mh_dictionary_new(state);
    state->greets = mh_dictionary_new(state);
}

unsigned short mh_word_add(Megahal *state, MegahalDict * dict, 
        MegahalString * word)
{
    int i, position;
    bool found;

    return (unsigned short)i;
}

MegahalDict * mh_words_split(Megahal * state, char * in)
{
    int i;
    MegahalDict * dict;
    MegahalString * str;
    char * token, * save, * p;

    i    = 0;
    save = mowgli_strdup(in);
    dict = mh_dictionary_new(state);
    p    = save;

    while ((token = strtok_r(NULL, " ", &save)) && token)
    {
        str = mowgli_alloc(sizeof(MegahalString));
        str->len = strlen(token);
        str->word = mowgli_strdup(token);
        dict->entry[i] = str;
        i++;
    }
    
    free(p);
    return dict;
}


MegahalTree * mh_node_new(Megahal * state)
{
    MegahalTree * node;

    node = mowgli_alloc(sizeof(MegahalTree));
    if (!node)
    {
        state->err("mh_node_new", "Allocation of new node failed");
        return NULL;
    }

    node->symbol  =  0;
    node->usage   =  0;
    node->count   =  0;
    node->branch  =  0;
    node->tree    =  NULL;

    return node;
}

inline void mh_ctx_init(MegahalModel * model)
{
    unsigned int i;
    for (i = 0; i <= model->order; ++i)
        model->ctx[i] = NULL;
}

void mh_model_destroy(Megahal * state, MegahalModel * model)
{
}

MegahalModel * mh_model_new(Megahal * state, int order)
{
    MegahalModel * model;

    model = mowgli_alloc(sizeof(MegahalModel));
    if (!model)
    {
        state->err("mh_model_new", "Allocation of new model failed");
        return NULL;
    }

    model->order = order;
    model->next = mh_node_new(state);
    model->prev = mh_node_new(state);
    model->ctx = malloc(sizeof(MegahalTree) * (order + 2));

    if (!model->ctx)
    {
        state->err("mh_model_new", "Allocation of context failed");
        mh_model_destroy(state, model);
        return NULL;
    }

    mh_ctx_init(model);
    model->dict = mh_dictionary_new(state);

    return model;
}

void mh_reply(Megahal * state, char * in, char * out, size_t n)
{
