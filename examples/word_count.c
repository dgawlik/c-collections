#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/vector.h"
#include "../include/hashmap.h"

struct kv {
    char* key;
    int value;
};

HASHMAP_DEFINE_ALL(char*, int, myhash);
VECTOR_DEFINE_ALL(struct kv, myvector);

int hash_code(char* str)
{
    int n = strlen(str);
    int hash = 7;

    for(int i=0;i<n;i++)
    {
        hash = 31*hash + (int)str[i];
    }

    return abs(hash);
}

int hash_equals(char* ls, char* rs)
{
    return strcmp(ls, rs) == 0;
}

char* fmt_int(int el)
{
    int length = snprintf( NULL, 0, "%d", el );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", el );
    return str;
}

char* fmt_str(char* str)
{
    return str;
}

int compare(struct kv lhs, struct kv rhs)
{
    return rhs.value - lhs.value;
}

int main (void) 
{
    char text[] = "In the first round of the tournament finals, the teams competed in eight round-robin groups of four teams for points, with the top two teams in each group proceeding. ";

    struct hashmap_context_myhash ctx = hashmap_init_myhash(hash_code, hash_equals);

    char split[] = " ,.- ";
    char * buf;
    buf = strtok(text, split);
    while(buf != NULL)
    {
        if(!hashmap_contains_key(&ctx, buf))
        {
            hashmap_put_myhash(&ctx, buf, 1);
        }
        else 
        {
            int c;
            hashmap_get_myhash(&ctx, buf, &c);
            hashmap_put_myhash(&ctx, buf, c+1);
        }

        buf = strtok(NULL, split);
    }

    char* repr = hashmap_to_string_myhash(&ctx, fmt_str, fmt_int);
    puts(repr);

    struct vector_context_myvector ctx2 = vector_init0_myvector();

    int len = hashmap_key_count_myhash(&ctx);
    char** keys = hashmap_keys_myhash(&ctx);
    for(int i=0;i<len;i++)
    {
        int value;
        hashmap_get_myhash(&ctx, keys[i], &value);

        struct kv pair = {
            .key = keys[i],
            .value = value
        };

        vector_push_last_myvector(&ctx2, pair);
    }

    vector_sort_myvector(&ctx2, compare);

    for(int i=0;i<3;i++)
    {
        struct kv pair;
        vector_get_myvector(&ctx2, i, &pair);
        puts(pair.key);
    }

    return 0;
}