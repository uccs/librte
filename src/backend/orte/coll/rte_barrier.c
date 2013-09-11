/*
 * Copyright (c) 2011-2013 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */
#include "orte_config.h"

#include "orte/runtime/orte_globals.h"
#include "rte.h"
#include "orte/mca/grpcomm/grpcomm.h"

int rte_orte_barrier (rte_group_t group)
{
    int rc;
    orte_grpcomm_collective_t *coll;

    coll = OBJ_NEW(orte_grpcomm_collective_t);
    coll->id = orte_grpcomm_base_get_coll_id ();// orte_process_info.peer_init_barrier;

    if (OPAL_SUCCESS != (rc = orte_grpcomm.barrier(coll))) {
        return RTE_ERROR;
    }

    coll->active = true;
    while (coll->active) {
        opal_progress();
    }

    OBJ_RELEASE(coll);

    return RTE_SUCCESS;
}
