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

#ifndef RTE_PMI_INTERNAL_H
#define RTE_PMI_INTERNAL_H

/* we hardcode this in the moment
 * TODO: put test in configure.ac to figure out if we have PMI on cray or slurm */
#define HAVE_CRAY_PMI 1 

#if HAVE_CRAY_PMI
typedef struct cray_pmi_proc_s {
    char* node_name;
} cray_pmi_proc_t;

extern cray_pmi_proc_t *rte_pmi_procs;
#endif

#endif /* RTE_PMI_INTERNAL_H */
