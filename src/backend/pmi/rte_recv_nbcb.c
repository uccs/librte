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

int rte_pmi_recv_nbcb  (rte_ec_handle_t src,
                        uint32_t tag,
                        int flags,
                        rte_group_t group,
                        rte_request_cb_t cb_fn,
                        void * cb_data)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

int rte_pmi_cancel(rte_ec_handle_t  peer, int tag)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

