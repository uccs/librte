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

rte_ec_handle_t rte_orte_group_index_to_ec(rte_group_t in_group,
                                      rte_node_index_t index)
{
    rte_proc_t *proc = (rte_proc_t *)opal_pointer_array_get_item(&rte_procs, index);
    if (NULL != proc)
        return &proc->name;
    else
        return NULL;
}

