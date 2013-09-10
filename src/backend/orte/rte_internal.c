#include "opal_config.h"
#include "orte_config.h"
#include "orte/types.h"
#include "opal/mca/hwloc/hwloc.h"
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
    proc->locality = RTE_PROC_LOCALITY_UNKNOWN;
}

static void rte_orte_proc_destruct(rte_proc_t* proc)
{
    if (NULL != proc && NULL != proc->hostname) {
       free(proc->hostname);
    }
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
    orte_vpid_t daemon;
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
            proc->hostname = strdup(orte_process_info.nodename);
        }
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
