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
#include "stci/communication/group.h"

RTE_PUBLIC int rte_stci_group_size(rte_group_t group)
{
    stci_agent_group_t *stci_group = NULL;

    if (NULL == group)
        return RTE_ERROR_BAD_INPUT;
    
    stci_group = (stci_agent_group_t *) group;

    /* For NULL pointer return negative group size */
    return !stci_group ? -1 : (int) stci_group->size;
}
