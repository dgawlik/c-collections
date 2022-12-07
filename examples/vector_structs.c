#include <stdlib.h>
#include <stdio.h>
#include "../include/vector.h"


struct packet {
    char* payload;
    int length;
    int is_broken;
};

VECTOR_DEFINE_ALL(struct packet, packet)


int only_valid(struct packet p)
{
    return !p.is_broken;
}

struct packet collect(struct packet l, struct packet r)
{
    struct packet n = {
        .payload = string_concat(l.payload, r.payload),
        .length = l.length + r.length,
        .is_broken = 0
    };

    return n;
}

int main(void)
{
    struct vector_context_packet ctx = vector_init0_packet();

    for(int i=0;i<10;i++)
    {
        struct packet pkt = {
            .payload = "blah",
            .length = 5,
            .is_broken = 0
        };

        if(i % 2 == 1)
        {
            pkt.is_broken = 1;
        }

        vector_push_last_packet(&ctx, pkt);
    }

    vector_filter_packet(&ctx, &only_valid);

    struct packet zero = {
        .is_broken = 0,
        .length = 0,
        .payload = ""
    };

    struct packet result = vector_reduce_packet(&ctx, zero, &collect);

    puts(result.payload);

    return 0;
}