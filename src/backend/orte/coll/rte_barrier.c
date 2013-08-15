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

#include "orte/runtime/orte_globals.h"
#include "rte.h"
#include "orte/mca/grpcomm/grpcomm.h"

int rte_orte_barrier (rte_group_t group)
{
    int rc;
    orte_grpcomm_collective_t coll;

    OBJ_CONSTRUCT(&coll, orte_grpcomm_collective_t);
    coll.id = orte_process_info.peer_init_barrier;
    opal_progress_event_users_increment();
    if (OPAL_SUCCESS != (rc = orte_grpcomm.barrier(&coll))) {
        return RTE_ERROR;
    }   
    while (coll.active) {
        rte_progress();
    }   
    opal_progress_event_users_decrement();
    return RTE_SUCCESS;
}
