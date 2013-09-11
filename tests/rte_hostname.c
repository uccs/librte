/*
 * Copyright (c)           2011 UT-Battelle, LLC. All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <stdio.h>
#include "rte.h"

#include "src/util/rte_util_locality.h"

int main (int argc, char** argv)
{
    int rc = RTE_SUCCESS;

    rte_ec_handle_t ec;
    rte_group_t     group;
    int             group_size, index, my_rank;
    rte_srs_session_t    session1  = NULL;

    char *locality_str;
    char *loc_str = "local";
    char *nloc_str = "not local";
    char *hostname;

    rte_init (&argc, &argv, &group);

    group_size = rte_group_size (group);

    my_rank =  rte_group_rank (group);

    if (0 > group_size)
        goto error;

    /* to get the locality we have to have a modex exchange first */
    rc = rte_srs_session_create (group, 0, &session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to allocate publish request\n");
        goto error;
    }

    rc = rte_srs_exchange_data (session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to prepare data publishing\n");
        goto error;
    }

    for (index = 0; index < group_size; index++) {

        ec = rte_group_index_to_ec (group, index);

        if (NULL == ec)
            goto error;

        hostname = rte_get_ec_hostname (ec);

        printf ("[rank:%d] rank %d is %s\n", my_rank, index, hostname);
    }

    rc = rte_srs_session_destroy(session1);
    if (RTE_SUCCESS != rc) {
        fprintf (stderr, "Error: Impossible to destroy srs session\n");
        goto error;
    }

    rte_finalize ();

    return rc;

error:
    /* rte_exit (); */
    printf ("error in %s", __FILE__);
    return rc;
}

