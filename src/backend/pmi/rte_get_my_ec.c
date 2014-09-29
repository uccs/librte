/*
 * Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "rte.h"

#include "rte_pmi_internal.h"

#include <pmi.h>
#include <stddef.h>
#include <stdio.h>

rte_ec_handle_t rte_pmi_get_my_ec(void)
{
    int rank, rc;
    rte_ec_handle_t ec_ptr;

    if (NULL == librte_pmi_procs) {
        fprintf(stderr, "rte_pmi_get_my_ec: Error, process arry is not initialized\n");
        return NULL;
    }

    rc = PMI_Get_rank (&rank);
    ec_ptr = librte_pmi_procs+rank;

    return ec_ptr;
}

