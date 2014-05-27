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
#include "opal/mca/db/db.h"
#include "opal/mca/hwloc/hwloc.h"

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

RTE_PUBLIC int rte_orte_get_ec_locality (rte_ec_handle_t ec_handle)
{
    int rc;
    orte_vpid_t daemon, *vptr;
    char *cpu_bitmap;

    orte_process_name_t * name = (orte_process_name_t *)ec_handle;

    rte_proc_t *proc = (rte_proc_t *)
        opal_pointer_array_get_item(&rte_procs, name->vpid);

    if (NULL == proc)
        return OPAL_PROC_LOCALITY_UNKNOWN;

    /* make sure we only check the db if we dont know about the locality  */
    if (RTE_PROC_LOCALITY_UNKNOWN == proc->locality) {
#if  OPAL_HAVE_HWLOC
        /* retrieve the binding for the other proc */
        rc = opal_db.fetch((opal_identifier_t*)&(proc->name), ORTE_DB_CPUSET,
                           (void*)&cpu_bitmap, OPAL_STRING);
        if (ORTE_SUCCESS != rc) {
            free(cpu_bitmap);
            fprintf (stderr, "Error: unable to query opal_db\n");
            return OPAL_PROC_LOCALITY_UNKNOWN;
        }

        vptr = &daemon;
        rc = opal_db.fetch((opal_identifier_t*)&(proc->name), ORTE_DB_DAEMON_VPID,
                           (void**)&vptr, OPAL_UINT32);
        if (ORTE_SUCCESS != rc) {
            free(cpu_bitmap);
            fprintf (stderr, "Error: unable to query opal_db\n");
            return OPAL_PROC_LOCALITY_UNKNOWN;
        }
        if (OPAL_EQUAL == orte_util_compare_name_fields(ORTE_NS_CMP_ALL,
            &proc->name, ORTE_PROC_MY_NAME)) {
            /* if this data is from myself, then set
             * locality to all */
            proc->locality = OPAL_PROC_ALL_LOCAL;
        } else if (daemon != ORTE_PROC_MY_DAEMON->vpid) {
            proc->locality = OPAL_PROC_NON_LOCAL;
        } else if (NULL == cpu_bitmap || NULL == orte_process_info.cpuset) {
            proc->locality = OPAL_PROC_ON_NODE;
        } else {
            /* we share a node - see what else we share */
            proc->locality = opal_hwloc_base_get_relative_locality(
                                                       opal_hwloc_topology,
                                                       orte_process_info.cpuset,
                                                       cpu_bitmap);
        }
#else
        if (OPAL_EQUAL == orte_util_compare_name_fields(ORTE_NS_CMP_ALL,
            &proc->name, ORTE_PROC_MY_NAME)) {
            proc->locality = OPAL_PROC_ALL_LOCAL;
        } else if (daemon != ORTE_PROC_MY_DAEMON->vpid) {
            proc->locality = OPAL_PROC_NON_LOCAL;
        } else {
            proc->locality = OPAL_PROC_ON_NODE;
        }
#endif
    }

    return proc->locality;
}
