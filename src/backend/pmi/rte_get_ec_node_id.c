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
    librte_pmi_proc_t *_ec_handle;

    if (NULL == ec_handle) {
        fprintf (stderr, "rte_pmi_get_ec_node_id: ec_handle is NULL\n");
        exit (-1);
    }

    if (NULL == librte_pmi_procs) {
        fprintf (stderr, "rte_pmi_get_ec_node_id: librte_pmi_procs is NULL\n");
        exit (-1);
    }

    fprintf(stderr, "rte_pmi_get_ec_node_id, rank is %d, (%p,%p)\n", rank, librte_pmi_procs, ec_handle);

#if HAVE_SLURM_PMI
    _ec_handle = ec_handle;
#endif

#if HAVE_CRAY_PMI
    _ec_handle = ec_handle;
#endif

    /* the names are initialized lazyly so check if it is initialized yet */
    rank = _ec_handle - librte_pmi_procs;
#if HAVE_CRAY_PMI
    rc = PMI_Get_nid (rank, &_nid);
#endif

#if HAVE_SLURM_PMI
    nid = 0
#endif

    return _nid;
}

