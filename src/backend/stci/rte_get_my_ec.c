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

rte_ec_handle_t rte_stci_get_my_ec(void)
{
    /* Pasha: It seem that the ec_handle is disconnected from
     group. I have to verify it with stci ppl */

    return (rte_ec_handle_t) &stci_process_self->name;
}
