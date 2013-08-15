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

RTE_PUBLIC int rte_orte_group_size(rte_group_t group)
{
    orte_proc_info_t * proc = (orte_proc_info_t *)group;

    if (NULL == proc)
        return -1;
    return proc->num_procs;
}
