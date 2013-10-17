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

rte_ec_handle_t rte_pmi_get_my_ec(void)
{
    int rank, rc;

    PMI_Get_rank (&rank);

    return (rte_ec_handle_t)(rte_pmi_procs+(rank*sizeof (cray_pmi_proc_t)));
}

