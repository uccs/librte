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
#include "opal/runtime/opal_progress.h"

int rte_orte_send (rte_iovec_t     *iov  ,
              uint32_t         cnt  ,
              rte_ec_handle_t  dst  ,
              uint32_t         tag  ,
              rte_group_t      group) 
{
    int rc;
    uint32_t vec_i;
    uint32_t c_i;
    size_t offset;
    opal_buffer_t* buffer = OBJ_NEW(opal_buffer_t);

    for (vec_i = 0; vec_i < cnt; ++vec_i) {
        offset = 0;
        for(c_i = 0; c_i < iov[vec_i].count; ++c_i) {
            rc = opal_dss.pack(buffer, 
                              (void *)((char *)iov[vec_i].iov_base + offset), 
                              1, (opal_data_type_t)iov[vec_i].type->opal_dt);
            if (OPAL_SUCCESS != rc) {
                return rc;
            }
            offset += get_datatype_size(iov[vec_i].type);
        }
    }

    opal_progress_event_users_increment();
    rc = orte_rml.send_buffer(dst, buffer, tag, 0);
    opal_progress_event_users_decrement();
    if (rc < 0) {
        return RTE_ERROR;
    }

    return RTE_SUCCESS;
}
