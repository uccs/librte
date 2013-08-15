#include "orte_config.h"
#include "orte/types.h"
#include "opal/mca/hwloc/base/base.h"
#include "opal/mca/db/db.h"
#include "orte/util/proc_info.h"
#include "orte/runtime/orte_globals.h"
#include "rte.h"
#include "rte_internal.h"

opal_pointer_array_t rte_procs;

static void rte_orte_proc_construct(rte_proc_t* proc)
{
    proc->hostname = NULL;
}

static void rte_orte_proc_destruct(rte_proc_t* proc)
{
    free(proc->hostname);
}

OBJ_CLASS_INSTANCE(
    rte_proc_t,
    opal_object_t,
    rte_orte_proc_construct,
    rte_orte_proc_destruct
);

int all_orte_proc_init(void)
{
    orte_vpid_t i;
    char *cpu_bitmap;
    int rc;

    OBJ_CONSTRUCT(&rte_procs, opal_pointer_array_t);
    rc = opal_pointer_array_init(&rte_procs, orte_process_info.num_procs, INT_MAX, 10);
    if (OPAL_SUCCESS != rc) {
        OBJ_DESTRUCT(&rte_procs);
        return RTE_ERROR;
    }

    /* create proc structures and find self */
    for( i = 0; i < orte_process_info.num_procs; i++ ) {
        rte_proc_t *proc = OBJ_NEW(rte_proc_t);
        rc = opal_pointer_array_set_item(&rte_procs, i, (void *)proc);
        if (OPAL_SUCCESS != rc) {
            OBJ_DESTRUCT(&rte_procs);
            return RTE_ERROR;
        }

        proc->name.jobid = orte_process_info.my_name.jobid;
        proc->name.vpid = i;

        if (i == orte_process_info.my_name.vpid) {
            proc->hostname = orte_process_info.nodename;
        } else {
            rc = opal_db.fetch_pointer((opal_identifier_t*)&(proc->name), 
                    ORTE_DB_HOSTNAME, 
                    (void**)&(proc->hostname), 
                    OPAL_STRING);
            if (ORTE_SUCCESS != rc)
                return RTE_ERROR;
        }

#if OPAL_HAVE_HWLOC
        /* retrieve the binding for the other proc */
        rc = opal_db.fetch((opal_identifier_t*)&(proc->name), 
                ORTE_DB_CPUSET, 
                (void**)&cpu_bitmap, OPAL_STRING);
        if (ORTE_SUCCESS != rc) {
            free(cpu_bitmap);
            return RTE_ERROR;
        }
        /* we share a node - see what else we share */
        proc->locality = opal_hwloc_base_get_relative_locality(opal_hwloc_topology,
                orte_process_info.cpuset,
                cpu_bitmap);
#else
        proc->locality = OPAL_PROC_ON_NODE;
#endif
    }

    return RTE_SUCCESS;
}

int all_orte_proc_release(void)
{
    orte_vpid_t i;

    /* create proc structures and find self */
    for( i = 0; i < orte_process_info.num_procs; i++ ) {
        rte_proc_t *proc;
        proc = (rte_proc_t *)opal_pointer_array_get_item(&rte_procs, i);
        if (NULL != proc) {
            OBJ_RELEASE(proc);
            return RTE_ERROR;
        }
    }
    OBJ_DESTRUCT(&rte_procs);
    return RTE_SUCCESS;
}
