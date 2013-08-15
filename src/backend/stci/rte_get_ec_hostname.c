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

#include <stdlib.h>
#include "rte.h"
#include "stci/runtime/stci_runtime.h"

/* FIXME */
RTE_PUBLIC char * rte_stci_get_ec_hostname(rte_ec_handle_t ec_handle) {
    STCI_Process_name_t *stci_proc_name =
        (struct STCI_Process_name_t *) ec_handle;

    if (stci_proc_name == NULL)
        goto error;

    /* XXX How can we get the STCI_Process_t structure behind the 
           STCI_Process_name_t? */

error:
    return "localhost";
}
