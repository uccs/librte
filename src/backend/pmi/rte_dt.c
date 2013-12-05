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

#include <string.h>


const struct rte_dt_t rte_int1       = {rte_pmi_int8, 8};
const struct rte_dt_t rte_int2       = {rte_pmi_int16, 16};
const struct rte_dt_t rte_int4       = {rte_pmi_int32, 32};
const struct rte_dt_t rte_int8       = {rte_pmi_int64, 64};
const struct rte_dt_t rte_uint1      = {rte_pmi_uint8, 8};
const struct rte_dt_t rte_uint2      = {rte_pmi_uint16, 16};
const struct rte_dt_t rte_uint4      = {rte_pmi_uint32, 32};
const struct rte_dt_t rte_uint8      = {rte_pmi_uint64, 64};
const struct rte_dt_t rte_float2     = {rte_pmi_float2, 64};
const struct rte_dt_t rte_bool       = {rte_pmi_bool, 8};

int rte_pmi_unpack (rte_iovec_t     *data,
                    void            *src,
                    uint32_t        *offset)
{
    int rc = RTE_SUCCESS;
    size_t dt_length;

    dt_length = get_datatype_size (data->type);
    memcpy (data->iov_base, src, dt_length);
    offset += dt_length;

    return rc;
}

size_t get_datatype_size(const struct rte_dt_t *dt) 
{
    return dt->size;
}
