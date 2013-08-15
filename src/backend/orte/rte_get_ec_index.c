/*
 * Copyright (c) 2012       Oak Rigde National Laboratory
 *                          All rights reserved.
 *
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

RTE_PUBLIC rte_node_index_t rte_orte_get_ec_index(rte_group_t     group    ,
                                             rte_ec_handle_t ec_handle) 
{
    orte_proc_info_t * proc = (orte_proc_info_t *)group;
    orte_process_name_t * name = ec_handle;

    if (proc->my_name.jobid != name->jobid)
        return -1;
    else 
        return name->vpid;
}
