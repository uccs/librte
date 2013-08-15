#include "rte.h"
#include "rte_dt.h"
#include "opal/datatype/opal_datatype_internal.h"

/* Build-in datatypes declaration 
 *
 * For initialization we use OPAL internal datatype macros...
 * it is not "kosher" but we C does not allow to use const for declaration */

const struct rte_dt_t rte_int1       = {OPAL_DATATYPE_INITIALIZER_INT1(0)  };
const struct rte_dt_t rte_int2       = {OPAL_DATATYPE_INITIALIZER_INT2(0)  };
const struct rte_dt_t rte_int4       = {OPAL_DATATYPE_INITIALIZER_INT4(0)  };
const struct rte_dt_t rte_int8       = {OPAL_DATATYPE_INITIALIZER_INT8(0)  };
const struct rte_dt_t rte_int16      = {OPAL_DATATYPE_INITIALIZER_INT16(0) };
const struct rte_dt_t rte_uint1      = {OPAL_DATATYPE_INITIALIZER_UINT1(0) };
const struct rte_dt_t rte_uint2      = {OPAL_DATATYPE_INITIALIZER_UINT2(0) };
const struct rte_dt_t rte_uint4      = {OPAL_DATATYPE_INITIALIZER_UINT4(0) };
const struct rte_dt_t rte_uint8      = {OPAL_DATATYPE_INITIALIZER_UINT8(0) };
const struct rte_dt_t rte_uint16     = {OPAL_DATATYPE_INITIALIZER_UINT16(0)};
const struct rte_dt_t rte_float2     = {OPAL_DATATYPE_INITIALIZER_FLOAT2(0)};
const struct rte_dt_t rte_float4     = {OPAL_DATATYPE_INITIALIZER_FLOAT4(0)};
const struct rte_dt_t rte_float8     = {OPAL_DATATYPE_INITIALIZER_FLOAT8(0)};
const struct rte_dt_t rte_float12    = {OPAL_DATATYPE_INITIALIZER_FLOAT12(0)};
const struct rte_dt_t rte_float16    = {OPAL_DATATYPE_INITIALIZER_FLOAT16(0)};
const struct rte_dt_t rte_complex8   = {OPAL_DATATYPE_INITIALIZER_COMPLEX8(0)};
const struct rte_dt_t rte_complex16  = {OPAL_DATATYPE_INITIALIZER_COMPLEX16(0)};
const struct rte_dt_t rte_complex32  = {OPAL_DATATYPE_INITIALIZER_COMPLEX32(0)};
const struct rte_dt_t rte_bool       = {OPAL_DATATYPE_INITIALIZER_BOOL(0)  };
const struct rte_dt_t rte_wchar      = {OPAL_DATATYPE_INITIALIZER_WCHAR(0) };
