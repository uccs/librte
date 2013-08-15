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
#include "include/rte.h"

RTE_PUBLIC int rte_orte_group_rank (rte_group_t group)
{
    orte_proc_info_t * proc = (orte_proc_info_t *)group;

    if (proc->my_name.jobid != orte_process_info.my_name.jobid)
        return -1;
    /* Only one group supported */
    return orte_process_info.my_name.vpid;
}
