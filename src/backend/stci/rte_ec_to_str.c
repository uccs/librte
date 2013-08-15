/*
 * Copyright (c) 2012       Oak Rigde National Laboratory
 *                          All rights reserved.
 *
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include "rte.h"
#include "stci/runtime/stci_runtime.h"

RTE_PUBLIC char * rte_stci_ec_to_str (rte_ec_handle_t a) {
    STCI_Process_name_t *aname = (STCI_Process_name_t*) a;
    return (stci_process_name_to_str (aname));
}

