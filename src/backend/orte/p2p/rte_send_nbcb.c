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
    int rc;
    uint32_t vec_i;
    uint32_t c_i;
    opal_buffer_t* buffer = OBJ_NEW(opal_buffer_t);

    for (vec_i = 0; vec_i < cnt; ++vec_i) {
        rc = opal_dss.pack(buffer, 
                          (void *)((char *)iov[vec_i].iov_base), 
                          iov[vec_i].count, (opal_data_type_t)iov[vec_i].type->opal_dt);
        if (OPAL_SUCCESS != rc) {
            return rc;
        }
    }

    rc = orte_rml.send_buffer_nb(dst, buffer, tag, 0, 
            /* Cast the function pointer to ORTE format */
            (void (*)(int status, struct orte_process_name_t* peer, 
                      struct opal_buffer_t* buffer, 
                      orte_rml_tag_t tag, void* cbdata)) cb_fn, cb_data);
    if (rc < 0) {
        return RTE_ERROR;
    }

    return RTE_SUCCESS;
}
