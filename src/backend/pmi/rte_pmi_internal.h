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

#include "rte_config.h"

#if HAVE_CRAY_PMI
    typedef struct cray_pmi_proc_s {
        char* node_name;
    } cray_pmi_proc_t;

    extern cray_pmi_proc_t *rte_pmi_procs;
    typedef cray_pmi_proc_t rte_pmi_proc_t;

    #define RTE_WANT_PMI2 1
#endif

#if HAVE_SLURM_PMI
    typedef struct slurm_pmi_proc_s {
        char* node_name;
    } slurm_pmi_proc_t;

    extern slurm_pmi_proc_t *rte_pmi_procs;
    typedef slurm_pmi_proc_t rte_pmi_proc_t;
    #define RTE_WANT_PMI2 0
#endif

typedef struct rte_pmi_srs_session_s {
    char* name;
} rte_pmi_srs_session_t;
typedef rte_pmi_srs_session_t *rte_pmi_srs_session_ptr_t;

/* we keep some data in this structure if we use pmi2  */
typedef struct rte_pmi2_info_s {
    int     size;
    int     rank;
    int     appnum;
    char    jobid[16];
} rte_pmi2_info_t;

extern rte_pmi2_info_t rte_pmi2_info;

#endif /* RTE_PMI_INTERNAL_H */
