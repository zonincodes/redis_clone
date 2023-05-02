#include <stddef.h>
#include <stdint.h>

// hashtable node, should be embedded into the payload

struct HNode
{
    HNode *next = NULL;
    uint64_t hcode = 0;
};

// a simple fixed-seized hashtable
struct HTab
{
    HNode **tab = NULL;
    size_t mask = 0;
    size_t size = 0;
};

// interface

struct HMap
{
    HTab ht1;
    HTab ht2;
    size_t resizing_pos = 0;
};

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*cmp)(HNode *, HNode *));
void hm_insert(HMap *hmap, HNode *node);
HNode *hm_pop(HMap *hmap, HNode *key, bool (*cmp)(HNode *, HNode *));