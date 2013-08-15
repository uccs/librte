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

RTE_PUBLIC char * rte_orte_get_ec_hostname(rte_ec_handle_t ec_handle) 
{
    return orte_get_proc_hostname(ec_handle);
}
