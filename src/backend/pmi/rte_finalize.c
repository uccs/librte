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
#if RTE_WANT_PMI2 == 0
    rc = PMI_Finalize ();
    if (PMI_SUCCESS == rc) {
        return RTE_SUCCESS;
    }
#else
    rc = PMI2_Finalize ();
    if (PMI_SUCCESS == rc) {
        return RTE_SUCCESS;
    }
#endif
    return RTE_ERROR;
}

