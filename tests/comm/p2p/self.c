/*
 * Copyright (c)           2011 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "rte.h"

int main (int argc, char** argv)
{
    int rc = RTE_SUCCESS;

    rte_group_t group;
    int my_rank;
    int group_size;

    char* value;

    char s_str[13] = "Hello World!";
    char r_buf[13];

    rte_iovec_t data;

    rte_ec_handle_t src, dst;

    data.type     = &rte_int1;
    data.count    = 13;

    value = getenv("RTE_DEBUG");

    if (NULL != value && 0 == strcmp(value, "1")) {
        char hostname[256];
        int value=1;
        gethostname (hostname, sizeof(hostname));
        printf ("Please attach debugger on hostname:%s to PID:%d, argc:%d",
                     hostname, getpid(), argc);
        while (value++) {
            sleep (1);
        }
    }

    rte_init (&argc, &argv, &group);

    my_rank    = rte_group_rank(group);
    group_size = rte_group_size(group);

    printf ("group size: %d, my rank is %d\n", group_size, my_rank);

    data.iov_base = &s_str;

    dst = rte_group_index_to_ec(group, my_rank);
    rte_send(&data, 1, dst, 11, group);
    data.iov_base = &r_buf;

    src = rte_group_index_to_ec(group, my_rank);
    rte_recv(&data, 1, src, 11, group);

    printf ("(%d) received \"%s\" from %d\n", my_rank, r_buf, 0);

    rte_finalize ();

    return rc;
}

