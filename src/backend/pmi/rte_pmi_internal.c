/*
 * Copyright (c) 2011-2013 UT-Battelle, LLC. All rights reserved.
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* this array holds the execution contects for all the ranks */
librte_pmi_proc_t *librte_pmi_procs;

rte_pmi2_info_t rte_pmi2_info;

int rte_pmi_proclist_init ()
{
    int process_group_size;
    size_t dt_size;
    /* get the size of the process group */
    PMI_Get_size (&process_group_size);

    dt_size = sizeof (librte_pmi_proc_t);

    /* allocate memory for the  ss structures */
    librte_pmi_procs = malloc (process_group_size * dt_size);

    /* initialize to 0 */
    memset (librte_pmi_procs, 0, process_group_size * sizeof (librte_pmi_proc_t));

    return RTE_SUCCESS;
}
