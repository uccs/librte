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

#include "stci/communication/group.h"

#include "rte.h"

RTE_PUBLIC int rte_stci_group_rank (rte_group_t group)
{
    if (NULL == group)
        return RTE_ERROR_BAD_INPUT;
    
    return ((stci_agent_group_t*)(group))->my_rank;
}
