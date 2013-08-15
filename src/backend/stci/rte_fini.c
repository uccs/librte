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

#include "rte.h"
#include "stci/agents/agent.h"
#include "stci/communication/group.h"

RTE_PUBLIC int rte_stci_finalize ()
{
    int rc;
  
    rc = agent_fini ();
    if (STCI_SUCCESS != rc) {
        return RTE_ERROR;
    }
    
    rc = stci_finalize ();
    if (STCI_SUCCESS != rc) {
        return RTE_ERROR;
    }

    return RTE_SUCCESS;
}
