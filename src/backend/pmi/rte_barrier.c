/*
 * Copyright (c)      2013 UT-Battelle, LLC. All rights reserved.
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

int rte_pmi_barrier (rte_group_t group)
{
    int rc;

    rc = PMI_Barrier ();
    if (PMI_SUCCESS == rc) {
        return RTE_SUCCESS;
    }

    return RTE_ERROR;
}

