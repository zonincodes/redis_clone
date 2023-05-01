#include <assert.h>
#include <stdlib.h>
#include "hashtable.h"

// n must be a power of 2

static void h_init(HTab *htab, size_t n)
{
    assert(n > 0 && ((n - 1) & n) == 0);
    htab->tab = (HNode **)calloc(sizeof(HNode *), n);
    htab->mask = n - 1;
    htab->size = 0;
}

// hashtable insertion
static void h_insert(HTab *htab, HNode *node)
{
    size_t pos = node->hcode & htab->mask;
    HNode *next = htab->tab[pos];
    node->next = next;
    htab->tab[pos] = node;
    htab->size++;
}

// hashtable look up subroutine.
static HNode **hm_lookup(
    HTab *htab, HNode *key, bool (*cmp)(HNode *, HNode *))
{
    if(!htab -> tab) return NULL;

    size_t pos = key->hcode & htab->mask;
    HNode **from = &htab ->tab[pos];
    while (*from){
        if(cmp(*from, key)) return from;
        from = &(*from)->next;
    }
    return NULL;
}

// remove a node from the chain
static HNode *h_detach(HTab *htab, HNode **from)
{
    HNode *node = *from;
    *from = (*from)->next;
    htab->size--;
    return node;
}

const size_t k_resizing_work = 128;

static void hm_help_resizing(HMap *hmap)
{
    if(hmap->ht2.tab == NULL)
    {
        return;
    }

    size_t nwork = 0;
    while(nwork < k_resizing_work && hmap->ht2.size > 0)
    {
        // scan for nodes from ht2 and move them to ht1
        HNode **from = &hmap->ht2.tab
        [hmap -> resizing_pos];
        if(!*from)
        {
            hmap->resizing_pos++;
            continue;
        }

        h_insert(&hmap->ht1, h_detach(&hmap->ht2, from));
        nwork++;
    }

    if(hmap->ht2.size == 0)
    {
        //done
        free(hmap->ht2.tab);
        hmap->ht2 = HTab{};
    }
}