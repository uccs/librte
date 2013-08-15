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
#include "orte/util/name_fns.h"

char * rte_orte_get_ec_node_name(rte_ec_handle_t ec_handle)
{
    /* Using hostname instead of nodename */
    return ORTE_NAME_PRINT(ec_handle);
}
