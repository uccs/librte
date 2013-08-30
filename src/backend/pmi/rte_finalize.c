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

RTE_PUBLIC int rte_pmi_finalize()
{
    int rc;

    rc = PMI_Finalize ();
    if (PMI_SUCCESS == rc) {
        return RTE_SUCCESS;
    }

    return RTE_ERROR;
}

