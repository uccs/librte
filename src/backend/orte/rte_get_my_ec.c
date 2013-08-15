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

#include "orte/util/proc_info.h"
#include "rte.h"

rte_ec_handle_t rte_orte_get_my_ec(void)
{
    return (rte_ec_handle_t) &orte_process_info.my_name;
}
