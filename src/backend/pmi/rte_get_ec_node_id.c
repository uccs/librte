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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


uint32_t rte_pmi_get_ec_node_id (rte_ec_handle_t ec_handle)
{
    uint32_t _nid;
    int rc, rank;
#if HAVE_SLURM_PMI
    rte_pmi_proc_t *_ec_handle = (slurm_pmi_proc_t*)ec_handle;
#endif

#if HAVE_CRAY_PMI
    rte_pmi_proc_t *_ec_handle = (cray_pmi_proc_t*)ec_handle;
#endif

    /* the names are initialized lazyly so check if it is initialized yet */
    rank = _ec_handle - rte_pmi_procs;
#if HAVE_CRAY_PMI
    rc = PMI_Get_nid (rank, &_nid);
#endif

#if HAVE_SLURM_PMI
    nid = 0
#endif

    return _nid;
}

