/*
 * Copyright (c) 2013        2011 UT-Battelle, LLC. All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "rte.h"

int main (int argc, char** argv)
{
    int rc      = RTE_SUCCESS;
    int retval  = EXIT_SUCCESS;
    char* value = NULL;

    rte_group_t         group;

    int my_rank, i;

    /* initialize the runtime  */
    rc = rte_init (&argc, &argv, &group);
    if (rc != RTE_SUCCESS) {
        printf ("Error: rte_init() failed\n");
        return (EXIT_FAILURE);
    }


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

    /* get my rank */
    my_rank = rte_group_rank (group);
    if (my_rank < 0) {
        printf ("Error: rte_group_rank() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }

    for (i = 0; i < 10; i++) {
        if (0 == my_rank) {
            printf ("Barrier: %d\n", i);
            fflush (stdout);
        }
        rc = rte_barrier(group);
        if (rc != RTE_SUCCESS) {
            printf("Error: rte_allgather() failed\n");
            retval = EXIT_FAILURE;
            goto clean_exit;
        }
    }

clean_exit:
    /* finalize the runtime */
    rte_finalize();

    return retval;
}

