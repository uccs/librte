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

#include "stci/communication/group.h"
#include "stci/runtime/stci_runtime.h"

/* we need to expose this through stci */
#include "opal/mca/hwloc/hwloc.h"

/* RTE locality types match up with hwloc locality types.  */

const int RTE_PROC_LOCALITY_UNKNOWN = OPAL_PROC_LOCALITY_UNKNOWN;
const int RTE_PROC_NON_LOCAL = OPAL_PROC_NON_LOCAL;
const int RTE_PROC_ON_CLUSTER = OPAL_PROC_ON_CLUSTER;
const int RTE_PROC_ON_CU = OPAL_PROC_ON_CU;
const int RTE_PROC_ON_NODE = OPAL_PROC_ON_NODE;
const int RTE_PROC_ON_BOARD = OPAL_PROC_ON_BOARD;
const int RTE_PROC_ON_NUMA = OPAL_PROC_ON_NUMA;
const int RTE_PROC_ON_SOCKET = OPAL_PROC_ON_SOCKET;
const int RTE_PROC_ON_L3CACHE = OPAL_PROC_ON_L3CACHE;
const int RTE_PROC_ON_L2CACHE = OPAL_PROC_ON_L2CACHE;
const int RTE_PROC_ON_L1CACHE = OPAL_PROC_ON_L1CACHE;
const int RTE_PROC_ON_CORE = OPAL_PROC_ON_CORE;
const int RTE_PROC_ON_HWTHREAD = OPAL_PROC_ON_HWTHREAD;
const int RTE_PROC_ALL_LOCAL = OPAL_PROC_ALL_LOCAL;

RTE_PUBLIC int rte_stci_get_ec_locality (rte_ec_handle_t ec_handle) {
    /*
     * Get information about ec locality,
     * So far Open MPI uses following set of locality flags:
     *
     * #define OPAL_PROC_ON_CLUSTER    0x10
     * #define OPAL_PROC_ON_CU         0x08
     * #define OPAL_PROC_ON_NODE       0x04
     * #define OPAL_PROC_ON_BOARD      0x02
     * #define OPAL_PROC_ON_SOCKET     0x01
     * #define OPAL_PROC_NON_LOCAL     0x00
     * #define OPAL_PROC_ALL_LOCAL     0x1f
     */

    stci_agent_set_t    *set;
    size_t               n_agents;
    int                  d, i;
    STCI_Process_t *aname;

    /* FIXME: we just cover the RTE_PROC_ON_NODE case right now */
    
    /* get the local process group */
    stci_agent_group_get_set (stci_group_local, &set, &n_agents);

    for (i = 0; i < opal_pointer_array_get_size (set); i++) {
        aname = opal_pointer_array_get_item (set, i);
        if (aname == NULL)
            continue;

        if (!stci_process_name_compare (&aname->name,
                                        (STCI_Process_name_t *)ec_handle,
                                        &d                              ))
            return RTE_PROC_ON_NODE;
    }

    return RTE_PROC_NON_LOCAL;
}
