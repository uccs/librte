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
#include "orte_config.h"

#include "rte_dt.h"
#include "opal/dss/dss.h"
#include "orte/runtime/orte_globals.h"
#include "rte.h"
#include "orte/mca/rml/rml.h"

const int RTE_RECV_REQUEST_PERSISTENT = ORTE_RML_PERSISTENT;
rte_ec_handle_t RTE_ANYSOURCE = (rte_ec_handle_t)(&orte_name_wildcard);

int rte_orte_recv_nbcb  (rte_ec_handle_t src,
                               uint32_t tag,
                               int flags,
                               rte_group_t group,
                               rte_request_cb_t cb_fn,
                               void * cb_data)
{
    int rc;

    rc = orte_rml.recv_buffer_nb(src, tag, flags, 
            /* Cast the function pointer to ORTE format */
            (void (*)(int status, struct orte_process_name_t* peer, 
                      struct opal_buffer_t* buffer, 
                      orte_rml_tag_t tag, void* cbdata)) cb_fn, cb_data);
    if (OPAL_SUCCESS != rc) {
        return RTE_ERROR;
    }

    return RTE_SUCCESS;
}

int rte_orte_cancel(rte_ec_handle_t  peer, int tag)
{
    int rc;
    
    rc = orte_rml.recv_cancel(peer, tag);
    if (OPAL_SUCCESS != rc) {
        return RTE_ERROR;
    }

    return RTE_SUCCESS;
}

