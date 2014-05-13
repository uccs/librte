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

int rte_orte_recv (rte_iovec_t      *iov  ,
              uint32_t          cnt  ,
              rte_ec_handle_t   src  ,
              uint32_t          tag  ,
              rte_group_t       group) 
{
    int rc;
    uint32_t vec_i;
    uint32_t c_i;
    uint32_t offset = 0;
    opal_buffer_t *buffer = OBJ_NEW(opal_buffer_t);;
    
    opal_progress_event_users_increment();
    rc = orte_rml.recv_buffer(src, buffer, tag, 0);
    opal_progress_event_users_decrement();
    if (rc < 0) {
        rc = RTE_ERROR;
        goto out;
    }

    for (vec_i = 0; vec_i < cnt; ++vec_i) {
        rc = rte_unpack (&iov[vec_i], buffer, &offset);
        if (RTE_SUCCESS != rc) {
            return rc;
        }
    }
    rc = RTE_SUCCESS;
out:
    OBJ_RELEASE(buffer);
    return rc;
}
