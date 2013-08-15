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
#include "opal/constants.h"
#include "orte/runtime/runtime.h"
#include "rte_internal.h"

RTE_PUBLIC int rte_orte_finilize()
{
    int rc;
    rc = orte_finalize();
    if (OPAL_SUCCESS != rc)
        return RTE_ERROR;
    return all_proc_release();
}
