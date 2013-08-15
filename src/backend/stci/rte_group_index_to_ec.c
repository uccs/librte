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

rte_ec_handle_t rte_stci_group_index_to_ec  (rte_group_t in_group  ,
                                        rte_node_index_t index)
{
    int rc;

    size_t                  n_agents;
    stci_agent_group_t     *group;
    stci_agent_set_t       *set;
    opal_pointer_array_t   *ptr_arr;

    void* hdl;

    /* some sanity checks first */
    if (NULL == in_group)
        goto error;

    /* cast rte_group_t to stci_agent_group_t * */
    group = (stci_agent_group_t*) in_group;

    rc = stci_agent_group_get_set (group, &set, &n_agents);
    if (STCI_SUCCESS != rc)
        goto error;

    ptr_arr = (opal_pointer_array_t*) set;

    hdl = opal_pointer_array_get_item (ptr_arr, index);

    return (rte_ec_handle_t) &((STCI_Process_t*)hdl)->name;

error:
    opal_output (0, "error in %s", __FILE__);
    return (rte_ec_handle_t) NULL;
}

