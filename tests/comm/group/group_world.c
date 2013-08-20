/*
 * Copyright (c) 2011       Oak Ridge National Laboratory.
 *                          All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include <stdio.h>
#include "rte.h"

int main (int argc, char **argv) {
    int i, rc;
    rte_group_t             group_world;
    rte_ec_handle_t         ec;
    
    rc = rte_init (&argc, &argv, &group_world);
    if (rc != RTE_SUCCESS) {
        printf ("rte_init() failed\n");
        return -1;
    }
    
    printf ("Group size: %d, My rank: %d\n", rte_group_size(group_world), rte_group_rank(group_world));
    for (i = 0; i < rte_group_size(group_world); i++) {
        ec = rte_group_index_to_ec (group_world, i);
        printf ("Rank %d is %s\n", i, rte_get_ec_node_name(ec)); 
    }

    rte_finalize();

    return 0;
}

