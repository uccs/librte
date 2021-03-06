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

#include <stdio.h>
#include "rte.h"

int main (int argc, char** argv)
{
    int rc = RTE_SUCCESS;

    rte_ec_handle_t ec;
    rte_group_t     group;
    int             group_size, index, loc, my_rank;

    char *locality_str;
    char  *loc_str = "local";
    char *nloc_str = "not local";

    rte_init (&argc, &argv, &group);

    group_size = rte_group_size (group);
    my_rank = rte_group_rank(group);

    if (0 > group_size)
        goto error;

    for (index = 0; index < group_size; index++) {

        ec = rte_group_index_to_ec (group, index);

        if (NULL == ec)
            goto error;

        loc = rte_get_ec_locality (ec);

        if (   RTE_PROC_ON_NODE == loc
            || RTE_PROC_ALL_LOCAL == loc) {
            locality_str = loc_str;
        } else {
            locality_str = nloc_str;
        }

        printf ("[%d]:rank %d is %s (loc: %x)\n", my_rank, index, locality_str, loc);
    }

    rte_finalize ();

    return rc;

error:
    /* rte_exit (); */
    printf ("error in %s", __FILE__);
    return rc;
}

