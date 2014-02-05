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

int rte_orte_send_nbcb(rte_iovec_t *iov,
                  uint32_t cnt,
                  rte_ec_handle_t dst,
                  uint32_t tag,
                  int flags,
                  rte_group_t group,
                  rte_request_cb_t cb_fn,
                  void * cb_data)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}
