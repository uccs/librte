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

#include "rte.h"
#include "stci/communication/request.h"
#include "stci/communication/p2p.h"

#include "stci/mca/p2pml/p2pml.h"

#include "stci/runtime/stci_runtime.h"
#include "stci/runtime/stci_progress.h"

#include "opal/class/opal_list.h"



/* this definitions are form orte/mca/rml/rml_types.h */
/* we need to mimic some of this behaviors for the recv_nb_functions */

enum {
/**
 * Non-persistent request that can be deleted when the request is
 * completed.  This is the default behavior.
 */
RTE_REQUEST_NON_PERSISTENT          = 0x00000000,

/**
 * flag to oob_recv to allow caller to peek a portion of the next
 * available message w/out removing the message from the queue.
 */     
RTE_REQUEST_PEEK                    = 0x00000001,

/**
 * flag to oob_recv to return the actual size of the message even if
 * the receive buffer is smaller than the number of bytes available
 */
RTE_REQUEST_RML_TRUNC               = 0x00000002,

/**
 * flag to oob_recv to request the oob to allocate a buffer of the
 * appropriate size for the receive and return the allocated buffer
 * and size in the first element of the iovec array.
 */
RTE_REQUEST_ALLOC                   = 0x00000004,

/**
 * posted non-blocking recv is persistent
 */
RTE_REQUEST_PERSISTENT              = 0x00000008,

/**
 * The request is a non-blocking request that can have its callback
 * triggered as soon as the request is completed, even if the OOB is
 * currently in the middle of another non-blocking request callback.
 */
RTE_REQUEST_FLAG_RECURSIVE_CALLBACK = 0x00000010
} RTE_REQUEST_TYPES;

const int RTE_RECV_REQUEST_PERSISTENT = RTE_REQUEST_PERSISTENT;

/*
 * FIXME: we need an actual implementation for this function or come up with a
 *        better solution.
 * FIXME: Statuses are not handled in the moment.
 */

void rte_stci_progress (void) {
    stci_progress();
}


/* note, this is a hack and needs to be fixed when there is a stci level implementation */
STCI_Process_name_t stci_anysource = { (STCI_JOBTYPE_TA << STCI_JOBID_BITS) | STCI_JOBID_WILDCARD , (STCI_VPIDEPOCH_WILDCARD << STCI_VPID_BITS) | STCI_VPID_WILDCARD};

rte_ec_handle_t RTE_ANYSOURCE = (rte_ec_handle_t)(&stci_anysource);


int rte_stci_unpack (rte_iovec_t     *data  ,
                void            *src   ,
                uint32_t        *offset)
{
    int rc = RTE_SUCCESS;
    
    stci_tlv_t *_data;
    _data = (stci_tlv_t *) data;
    rc = stci_p2pml.unpack (_data, src, offset);
    
    return rc;
}

int rte_stci_cancel (rte_ec_handle_t  peer, int tag)
{
    int rc = RTE_SUCCESS;
    opal_list_t *requests;
    
    stci_p2pml.find_requests((STCI_Process_name_t*)peer, tag, &requests);
    
    opal_list_item_t                *item;
    STCI_Request_t *r;
    
    /* we iterate over all posted requests and try to find a matching one */
    for (item =  opal_list_get_first (requests);
         item != opal_list_get_end   (requests);
         item =  opal_list_get_next  (item)             )
    {
        r = (STCI_Request_t*) item;
        if (NULL != ((STCI_Request_t*)r)->cancel_fn) {
            rc = (r)->cancel_fn (r);
        }
    }
    
    return rc;
}
