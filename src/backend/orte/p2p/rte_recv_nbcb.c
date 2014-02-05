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
    return RTE_ERROR_NOT_IMPLEMENTED;
}

int rte_orte_cancel(rte_ec_handle_t  peer, int tag)
{
    int rc;

    orte_rml.recv_cancel(peer, tag);

    return RTE_SUCCESS;
}

