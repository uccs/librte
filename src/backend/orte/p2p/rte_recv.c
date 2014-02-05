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
    return RTE_ERROR_NOT_IMPLEMENTED;
}
