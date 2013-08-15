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
#include "stci/communication/group.h"
#include "stci/runtime/stci_runtime.h"
#include "stci/session.h"

char * rte_stci_get_session_dir(rte_ec_handle_t ec_handle)
{
    /* GV Map to the STCI function */
    return (get_session_dir ()); 

#if 0
    /* Pasha: I have to figure out how to access session directory info, sort of non trivial*/ 
    return NULL == getenv("TMPDIR") ? "/tmp" : getenv("TMPDIR");
#endif
}
