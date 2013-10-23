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

RTE_PUBLIC int rte_pmi_init(int *argc, char ***argv, rte_group_t *out_group)
{
   int rc = RTE_SUCCESS, spawned, size, rank, appnum;

#if RTE_WANT_PMI2 == 0
   rc = PMI_Init (&spawned);
   if (PMI_SUCCESS != rc) {
       return RTE_ERROR;
   }
#else  
   rc = PMI2_Init (&spawned, &rte_pmi2_info.size, &rte_pmi2_info.rank,
                   &rte_pmi2_info.appnum);
   if (PMI_SUCCESS != rc) {
       return RTE_ERROR;
   }

   rc = PMI2_Job_GetId (rte_pmi2_info.jobid, 16);
#endif

   /* set up the RTE proc table */
   rc = rte_pmi_proclist_init ();
   
   return rc;
}

