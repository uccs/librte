/*
 * Copyright (c) 2013       UT-Battelle, LLC. All rights reserved.
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
#include "rte_pmi_internal.h"

#include <pmi.h>

RTE_PUBLIC rte_node_index_t rte_pmi_get_ec_index(rte_group_t     group,
                                                 rte_ec_handle_t ec_handle)
{
#if HAVE_CRAY_PMI
    cray_pmi_proc_t *_ec_handle = (cray_pmi_proc_t*)ec_handle;
#endif

#if HAVE_SLURM_PMI
    slurm_pmi_proc_t *_ec_handle = (slurm_pmi_proc_t*)ec_handle; 
#endif

    return _ec_handle - rte_pmi_procs;
}

