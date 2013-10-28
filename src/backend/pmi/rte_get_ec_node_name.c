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

#include <limits.h>
#include <pmi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


char * rte_pmi_get_ec_node_name(rte_ec_handle_t ec_handle)
{
    rte_pmi_proc_t *_ec_handle = (slurm_pmi_proc_t*)ec_handle;

    /* the names are initialized lazyly so check if it is initialized yet */
    if (NULL == _ec_handle->node_name) {
        int rank, nid, rc;
	printf ("proc base pointer = %p, ec_handle is %p\n", rte_pmi_procs, ec_handle);
        /* get the rank */
        rank = _ec_handle - rte_pmi_procs;
#if HAVE_CRAY_PMI
        rc = PMI_Get_nid (rank, &nid);
	_ec_handle->node_name = malloc (16);
        sprintf (_ec_handle->node_name, "nid%d", nid);
#endif

#if HAVE_SLURM_PMI
	_ec_handle->node_name = malloc (HOST_NAME_MAX);
        gethostname (_ec_handle->node_name, HOST_NAME_MAX);
#endif
    }
    return _ec_handle->node_name;
}

