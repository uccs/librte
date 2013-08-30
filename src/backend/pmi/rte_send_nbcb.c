/*
 * Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "rte.h"

int rte_pmi_send_nbcb(rte_iovec_t *iov,
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

