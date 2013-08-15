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

RTE_PUBLIC int rte_stci_init(int *argc, char ***argv, rte_group_t *out_group)
{
    int rc;
    /* PSH: we have update the interface to return the handle */
    rc = stci_init(STCI_JOBTYPE_INVALID, argc, argv);
    if (STCI_SUCCESS != rc) {
        *out_group = NULL;
        return RTE_ERROR;
    }

    /* PSH: Agent interface seems to be fine */
    rc = agent_init();
    if (STCI_SUCCESS != rc) {
        *out_group = NULL;
        return RTE_ERROR;
    }

    *out_group = (rte_group_t*) stci_group_all;

    return RTE_SUCCESS;
}
