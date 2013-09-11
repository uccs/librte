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

#include "orte_config.h"

#include "rte.h"
#include "orte/constants.h"
#include "orte/runtime/runtime.h"
#include "rte_internal.h"

RTE_PUBLIC int rte_orte_finalize()
{
    int rc;
    rc = orte_finalize();
    if (ORTE_SUCCESS != rc)
        return RTE_ERROR;

    rc = all_orte_proc_release();
    if (RTE_SUCCESS != rc) {
        return RTE_ERROR;
    }
}
