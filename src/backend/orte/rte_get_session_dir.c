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
#include "orte/util/proc_info.h"
#include "rte.h"

char * rte_orte_get_session_dir(rte_ec_handle_t ec_handle)
{
    return orte_process_info.job_session_dir; 
}
