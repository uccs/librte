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
#include "rte_internal.h"

const int RTE_PROC_LOCALITY_UNKNOWN = OPAL_PROC_LOCALITY_UNKNOWN;
const int RTE_PROC_NON_LOCAL =        OPAL_PROC_NON_LOCAL;
const int RTE_PROC_ON_CLUSTER =       OPAL_PROC_ON_CLUSTER;
const int RTE_PROC_ON_CU =            OPAL_PROC_ON_CU;
const int RTE_PROC_ON_NODE =          OPAL_PROC_ON_NODE;
const int RTE_PROC_ON_BOARD =         OPAL_PROC_ON_BOARD;
const int RTE_PROC_ON_NUMA =          OPAL_PROC_ON_NUMA;
const int RTE_PROC_ON_SOCKET =        OPAL_PROC_ON_SOCKET;
const int RTE_PROC_ON_L3CACHE =       OPAL_PROC_ON_L3CACHE;
const int RTE_PROC_ON_L2CACHE =       OPAL_PROC_ON_L2CACHE;
const int RTE_PROC_ON_L1CACHE =       OPAL_PROC_ON_L1CACHE;
const int RTE_PROC_ON_CORE =          OPAL_PROC_ON_CORE;
const int RTE_PROC_ON_HWTHREAD =      OPAL_PROC_ON_HWTHREAD;
const int RTE_PROC_ALL_LOCAL =        OPAL_PROC_ALL_LOCAL;

RTE_PUBLIC int rte_orte_get_ec_locality (rte_ec_handle_t ec_handle) {

    orte_process_name_t * name = (orte_process_name_t *)ec_handle;

    rte_proc_t *proc = (rte_proc_t *)
        opal_pointer_array_get_item(&rte_procs, name->vpid);

    if (NULL != proc)
        return OPAL_PROC_LOCALITY_UNKNOWN;

    return proc->locality;
}
