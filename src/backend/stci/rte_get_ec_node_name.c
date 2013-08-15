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
#include "stci/runtime/stci_runtime.h"

char * rte_stci_get_ec_node_name(rte_ec_handle_t ec_handle)
{
    struct STCI_Process_name_t *stci_proc_name = 
        (struct STCI_Process_name_t *) ec_handle;
    
    return stci_process_name_to_str(stci_proc_name);
}
