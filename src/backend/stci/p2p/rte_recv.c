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

int rte_stci_recv (rte_iovec_t      *iov  ,
              uint32_t          cnt  ,
              rte_ec_handle_t   src  ,
              uint32_t          tag  ,
              rte_group_t       group) 
{
    int rc;
    
    stci_tlv_t          *d = (stci_tlv_t*) iov;
    STCI_Process_name_t *s = src;
    stci_agent_group_t  *g = group;
    
    
    rc = stci_p2pml.recv (d, cnt, s, tag, g);
    
    return rc;
}
