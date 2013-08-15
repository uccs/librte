/*
 * Copyright (c) 2011-2012   Oak Ridge National Laboratory
 *                           All Rights Reserved.
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
    rte_ec_handle_t     my_ec;
    
    int group_size, my_rank, i;
    rte_iovec_t *data_vec;
    int* data;
    
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
    
    /* get the group size */
    group_size = rte_group_size (group);
    if (group_size < 0) {
        printf ("Error: rte_group_size() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }

    /* get my ec */
    my_ec = rte_get_my_ec ();
    if (my_ec == NULL) {
        printf ("Error: rte_get_my_ec() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }

    /* get my rank */
    my_rank = rte_group_rank (group);
    if (my_rank < 0) {
        printf ("Error: rte_group_rank() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }

    /* allocate memory for the data */
    data = malloc (sizeof(int) * group_size);
    if (data == NULL) {
        printf ("Error: malloc() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }
    
    /* allocate memory for all ecs iovecs */
    data_vec = (rte_iovec_t*) malloc (group_size * sizeof(rte_iovec_t));
    if (data_vec == NULL) {
        printf ("Error: malloc() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }
    
    /* initialize the vectors */
    for (i = 0; i < group_size; i++) {
        data_vec[i].type     = &rte_int4;
        data_vec[i].count    = 1;
        data_vec[i].iov_base = &(data[i]);
        data[i] = -1;
    }
    
    data[my_rank] = my_rank;
    
    rc = rte_allgather(group, 
                       &data_vec[my_rank], 
                       data_vec, 
                       group_size);
    if (rc != RTE_SUCCESS) {
        printf("Error: rte_allgather() failed\n");
        retval = EXIT_FAILURE;
        goto clean_exit;
    }

    if (0 == my_rank) {
        for (i = 0; i < group_size; i++) {
            printf("(%d) %d->%d",my_rank, i, 
                        *((int*)(data_vec[i].iov_base)) );
        }
    }
    
    printf ("%d -> all done", my_rank);
    
clean_exit:
    /* finalize the runtime */
    rte_fini();

    return retval;
}

