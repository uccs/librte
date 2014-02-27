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

static int coll_id = 1;

int rte_orte_barrier (rte_group_t group)
{
    int rc;
    orte_grpcomm_collective_t coll;

    OBJ_CONSTRUCT(&coll, orte_grpcomm_collective_t);
//    coll.id = ++coll_id;
    if (0 == coll_id) {
        coll.id = orte_process_info.peer_modex;
    } else if (1 == coll_id) {
        coll.id = orte_process_info.peer_init_barrier;
    } else {
        coll.id = orte_process_info.peer_fini_barrier;
    }
    coll.active = true;
    coll_id++;

    if (OPAL_SUCCESS != (rc = orte_grpcomm.barrier(&coll))) {
        return RTE_ERROR;
    }

    while (coll.active) {
        opal_progress();
    }

    return RTE_SUCCESS;
}
