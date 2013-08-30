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
#include <pmi.h>

RTE_PUBLIC int rte_pmi_group_rank (rte_group_t group)
{
    int rc, rank;

    rc = PMI_Get_rank (&rank);
    if (PMI_SUCCESS == rc) {
        return rank;
    }

    return RTE_ERROR;
}

