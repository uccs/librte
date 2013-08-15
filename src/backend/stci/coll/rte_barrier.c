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

#include "rte.h"

#include <stdlib.h>
#include "stci/communication/coll.h"
#include "stci/runtime/stci_runtime.h"

int rte_stci_barrier (rte_group_t group)
{
    int rc;
    
    /* the barrier is an allgather with 0 length messages */
    stci_p2p_fn_handle_t table;
    
    rc = stci_setup_default_p2p_fn_table (&table);
    if (STCI_SUCCESS != rc) {
        return RTE_ERROR;
    }
    
    rc = stci_barrier (group, &table);
    if (STCI_SUCCESS != rc) {
        return RTE_ERROR;
    }
    
    return RTE_SUCCESS;
}
