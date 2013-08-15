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

#include "rte.h"

#include "stci/communication/group.h"

/* FIXME */
RTE_PUBLIC rte_node_index_t rte_stci_get_ec_index(rte_group_t     group    ,
                                             rte_ec_handle_t ec_handle) {
    return stci_agent_group_ec_to_index (group, ec_handle);
}
