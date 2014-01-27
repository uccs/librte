/*
 * Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "rte.h"
#include "rte_pmi_internal.h"

#include <pmi.h>
#include <stddef.h>

rte_ec_handle_t rte_pmi_group_index_to_ec(rte_group_t in_group,
                                          rte_node_index_t index)
{
    if (index > rte_group_size(in_group)) return NULL;
    return (rte_ec_handle_t)(rte_pmi_procs+index);
}


