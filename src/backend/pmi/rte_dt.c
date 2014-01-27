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
#include <stdio.h>


const struct rte_dt_t rte_int1       = {rte_pmi_int8, 1};
const struct rte_dt_t rte_int2       = {rte_pmi_int16, 2};
const struct rte_dt_t rte_int4       = {rte_pmi_int32, 4};
const struct rte_dt_t rte_int8       = {rte_pmi_int64, 8};
const struct rte_dt_t rte_uint1      = {rte_pmi_uint8, 1};
const struct rte_dt_t rte_uint2      = {rte_pmi_uint16, 2};
const struct rte_dt_t rte_uint4      = {rte_pmi_uint32, 4};
const struct rte_dt_t rte_uint8      = {rte_pmi_uint64, 8};
const struct rte_dt_t rte_float2     = {rte_pmi_float2, 8};
const struct rte_dt_t rte_bool       = {rte_pmi_bool, 1};

static int prep_buffer (char* current, char** next)
{
    char* tmp;

    tmp = strchr (current, ',');
    *next = tmp + 1;
    
    return RTE_SUCCESS;
}

int rte_pmi_unpack (rte_iovec_t     *data,
                    void            *src,
                    uint32_t        *offset)
{
    int rc = RTE_SUCCESS, j;
    size_t dt_length;
    char *base_ptr = NULL;
    char *data_ptr = NULL;

    base_ptr = ((char*)(src)) + *offset;
    data_ptr = data->iov_base;
    
    for (j=0; j < data->count; j++) {
            int nb;
            char* tmp;
            switch (data->type->type) {
                case rte_pmi_bool:
                case rte_pmi_int8:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%d", ((int8_t*)(data_ptr)));
                    base_ptr = tmp;
                    data_ptr++;
                    break;
                case rte_pmi_uint8:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%u", (uint8_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr++;
                    break;
                case rte_pmi_int16:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%d", (int16_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=2;
                    break;
                case rte_pmi_uint16:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%u", (uint16_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=2;
                    break;
                case rte_pmi_int32:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%d", (int32_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=4;
                    break;
                case rte_pmi_uint32:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%u", (uint32_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=4;
                    break;
                case rte_pmi_int64:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%ld", (int64_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=8;
                    break;
                case rte_pmi_uint64:
                    prep_buffer (base_ptr, &tmp);
                    nb = sscanf (base_ptr, "%lu", (uint64_t*)(data_ptr));
                    base_ptr = tmp;
                    data_ptr+=8;
                    break;
                case rte_pmi_float2:
                    break;
            }
        }


    *offset = base_ptr - (char*)src;

    return rc;
}

size_t get_datatype_size(const struct rte_dt_t *dt) 
{
    return dt->size;
}
