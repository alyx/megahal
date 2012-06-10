#include "mh.h"

static void make_caps(char *key)
{
    int i;
    for (i = 0; key[i] != '\0'; ++i)
        key[i] = toupper(key[i]);
}

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

    state->words = mowgli_patricia_create(make_caps);
}

unsigned short mh_word_add(Megahal *state, MegahalDict * dict, 
        MegahalString * word)
{
    int i, position;
    bool found;

    return (unsigned short)i;
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

void mh_reply(Megahal * state, char * in, char * out, size_t n);
