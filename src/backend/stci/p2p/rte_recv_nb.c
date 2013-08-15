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
#include "stci/mca/am/am.h"

#include "rte.h"
#include "rte_request.h"

void rte_stci_recv_nbcb_complete_fn (struct STCI_Request_t *req        ,
                                int                    status     ,
                                int                    tag        ,
                                STCI_Process_name_t   *peer       ,
                                void                  *data_buffer,
                                void                  *context    )
{
    rte_request_data_t *rte_cb_data;
    
    /* get the context */
    rte_cb_data = (rte_request_data_t*)context;
    
    /* call the callback */
    
    rte_cb_data->cb_fn (status, (rte_ec_handle_t)peer,
                        data_buffer, tag, rte_cb_data->cb_data);
    
    /* we need to free the request if it is not persistent */
    if ( STCI_REQUEST_RECV == req->type) {
        stci_free_request (req);
        req = NULL;
    }
    
    return;
}

int rte_stci_recv_nbcb (rte_ec_handle_t      src,
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
    rte_request_data_t *rte_cb_data = NULL;
    
    if (flags == RTE_RECV_REQUEST_PERSISTENT) {
        type = STCI_REQUEST_RECV_PERSISTENT;
    } else {
        type = STCI_REQUEST_RECV;
    }
    
    rte_cb_data = malloc (sizeof(rte_request_data_t));
    
    /* set the data for the request */
    rte_cb_data->cb_fn = cb_fn;
    rte_cb_data->cb_data = cb_data;

    /* allocate a request */
    rc = stci_p2pml.allocate (tag, type,
                              &rte_stci_recv_nbcb_complete_fn,
                              rte_cb_data, group, &request);
    if (STCI_SUCCESS != rc) {
        return RTE_ERROR;
    }
    
    rc = stci_p2pml.post_recv (request, src);
    
    return RTE_SUCCESS;
}