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

RTE_PUBLIC int rte_pmi_group_size(rte_group_t group)
{
    int rc, size;

    rc = PMI_Get_size (&size);
    if (PMI_SUCCESS == rc) {
        return size;
    }

    return RTE_ERROR;
}

