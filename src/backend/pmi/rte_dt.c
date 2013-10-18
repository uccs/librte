/*
 * Copyright (c)      2013 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include "rte.h"
#include "rte_dt.h"

int rte_pmi_unpack (rte_iovec_t     *data,
                    void            *src,
                    uint32_t        *offset)
{
    int rc = RTE_SUCCESS;

    return rc;
}

size_t get_datatype_size(const struct rte_dt_t *dt) 
{
    return dt->size;
}
