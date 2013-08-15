#include "orte_config.h"

#include "rte_dt.h"
#include "rte.h"
#include "orte/runtime/orte_globals.h"
#include "opal/dss/dss.h"
#include "opal/dss/dss_types.h"

/* Build-in datatypes declaration 
 *
 * For initialization we use OPAL internal datatype macros...
 * it is not "kosher" but we C does not allow to use const for declaration */

const struct rte_dt_t rte_int1       = {OPAL_INT8};
const struct rte_dt_t rte_int2       = {OPAL_INT16};
const struct rte_dt_t rte_int4       = {OPAL_INT32};
const struct rte_dt_t rte_int8       = {OPAL_INT64};
const struct rte_dt_t rte_uint1      = {OPAL_UINT8};
const struct rte_dt_t rte_uint2      = {OPAL_UINT16};
const struct rte_dt_t rte_uint4      = {OPAL_UINT32};
const struct rte_dt_t rte_uint8      = {OPAL_UINT64};
const struct rte_dt_t rte_float2     = {OPAL_FLOAT};
const struct rte_dt_t rte_bool       = {OPAL_BOOL};

int rte_orte_unpack (rte_iovec_t     *data  ,
                void            *src   ,
                uint32_t        *offset)
{
    int rc;
    uint32_t c_i;
    opal_buffer_t * buffer = (opal_buffer_t *) src;
    size_t buffer_offset = 0;
    
    for(c_i = 0; c_i < data->count; ++c_i) {
        rc = opal_dss.unpack(buffer, (void *)((char *)data->iov_base + buffer_offset), 
                /* Pasha: I dont like the int32_t casting */
                (int32_t *)offset, data->type->opal_dt);
        if (OPAL_SUCCESS != rc) {
            rc = RTE_ERROR;
            goto out;
        }
        buffer_offset += get_datatype_size(data->type);
    }
    rc = RTE_SUCCESS;
out:
    return rc;
}

size_t get_datatype_size(const struct rte_dt_t *dt) 
{
    int dds_dt = (dt->opal_dt);
    switch(dds_dt) {
        case OPAL_INT8 :
            return sizeof(int8_t);
        case OPAL_INT16 :
            return sizeof(int16_t);
        case OPAL_INT32 :
            return sizeof(int32_t);
        case OPAL_INT64 :
            return sizeof(int64_t);
        case OPAL_UINT8 :
            return sizeof(uint8_t);
        case OPAL_UINT16 :
            return sizeof(uint16_t);
        case OPAL_UINT32 :
            return sizeof(uint32_t);
        case OPAL_UINT64:
            return sizeof(uint64_t);
        case OPAL_FLOAT:
            return sizeof(float);
        case OPAL_BOOL :
            return sizeof(bool);
    }
    return 0;
}
