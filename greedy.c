#include <stdio.h>
#include <stdlib.h>

typedef struct setnode setnode_t;
struct setnode {
        setnode_t *next;
        long long item_val;
};
typedef struct setlist setlist_t;
struct setlist {
        setnode_t *head;
        long long sum;
};
setlist_t *setlist_malloc(void);
void setlist_push(setlist_t *sl, long long item_val);
void setlist_free(setlist_t *sl);
void setlist_print(const setlist_t *sl);

typedef struct llarray llarray_t;
struct llarray {
        size_t count;
        long long *elems;
};

int llong_dsc_cmp(const void *a, const void *b);

int main(void) {
        llarray_t items;
        scanf(" %zu", &items.count);
        items.elems = malloc(items.count*sizeof(*items.elems));
        for (size_t i=0; i<items.count; i++) {
                scanf(" %lld", items.elems+i);
        }
        qsort(items.elems, items.count, sizeof(*items.elems), llong_dsc_cmp);
        setlist_t *set0 = setlist_malloc();
        setlist_t *set1 = setlist_malloc();
        for (size_t i=0; i<items.count; i++) {
                if (set0->sum <= set1->sum) {
                        setlist_push(set0, items.elems[i]);
                } else {
                        setlist_push(set1, items.elems[i]);
                }
        }
        printf("set0:\n");
        setlist_print(set0);
        printf("set1:\n");
        setlist_print(set1);
        printf("sum diff = %lld\n", (set0->sum < set1->sum)
                                    ? set1->sum - set0->sum
                                    : set0->sum - set1->sum);
        setlist_free(set0);
        setlist_free(set1);
        free(items.elems);
        return 0;
}

setlist_t *setlist_malloc(void) {
        setlist_t *tmp = malloc(sizeof(*tmp));
        tmp->head = NULL;
        tmp->sum = 0;
        return tmp;
}
void setlist_push(setlist_t *sl, long long item_val) {
        setnode_t *next = sl->head;
        sl->head = malloc(sizeof(*sl->head));
        sl->head->next = next;
        sl->head->item_val = item_val;
        sl->sum += item_val;
}
void setlist_free(setlist_t *sl) {
        setnode_t tmp;
        tmp.next = sl->head;
        while (tmp.next != NULL) {
                setnode_t *next = tmp.next->next;
                free(tmp.next);
                tmp.next = next;
        }
        free(sl);
}
void setlist_print(const setlist_t *sl) {
        printf("set sum: %lld\n"
               "set items:\n",
               sl->sum);
        setnode_t tmp;
        tmp.next = sl->head;
        while (tmp.next != NULL) {
                printf("%lld ", tmp.next->item_val);
                tmp.next = tmp.next->next;
        }
        putchar('\n');
}

int llong_dsc_cmp(const void *a, const void *b) {
        long long av = *(long long *)a;
        long long bv = *(long long *)b;
        return (av > bv)*(-1) + (av < bv)*(1);
}
