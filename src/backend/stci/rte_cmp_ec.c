/*
 * Copyright (c) 2011      Oak Rigde National Laboratory
 *                         All rights reserved.
 */

#include "rte.h"
#include "stci/runtime/stci_runtime.h"

int rte_stci_cmp_ec (rte_ec_handle_t ec_handle_one, rte_ec_handle_t ec_handle_two)
{
    int diff;

    return (stci_process_name_compare ((STCI_Process_name_t*)ec_handle_one,
                                       (STCI_Process_name_t*)ec_handle_two,
                                       &diff));
}

