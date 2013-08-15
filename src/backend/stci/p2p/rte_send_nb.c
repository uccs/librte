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

#include "stci/communication/p2p.h"

#include "stci/mca/p2pml/p2pml.h"

#include "rte.h"
#include "rte_request.h"

int rte_stci_send_nbcb (rte_iovec_t         *iov,
                   uint32_t             cnt,
                   rte_ec_handle_t      dst,
                   uint32_t             tag,
                   int                  flags,
                   rte_group_t          group,
                   rte_request_cb_t     cb_fn,
                   void                *cb_data)
{
    int rc;
    STCI_Request_t *request;
    int type;
    stci_request_complete_fn_t cb;
    
    if (flags == RTE_RECV_REQUEST_PERSISTENT) {
        return RTE_ERROR_BAD_INPUT;
    }

    /* We are cheating here a little. STCI does not really support non blocking
       sends at the moment anyway so we just go with a locking one and call the
       callback from here  */
    
    stci_tlv_t          *d = (stci_tlv_t*) iov;
    STCI_Process_name_t *s = dst;
    stci_agent_group_t  *g = group;
    
    rc = stci_p2pml.send (d, cnt, s, tag, g);
    
    cb_fn (RTE_SUCCESS, dst, NULL, tag, cb_data);
    
    return rc;
}