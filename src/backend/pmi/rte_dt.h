/*
 * Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#ifndef RTE_DT_H
#define RTE_DT_H

#include <unistd.h>

typedef enum {
    rte_pmi_bool = 1,
    rte_pmi_int8,
    rte_pmi_int16,
    rte_pmi_int32,
    rte_pmi_int64,
    rte_pmi_uint8,
    rte_pmi_uint16,
    rte_pmi_uint32,
    rte_pmi_uint64,
    rte_pmi_float2,
} rte_pmi_dt_type_t;

struct rte_dt_t {
    rte_pmi_dt_type_t type;
    int               size;
};

size_t get_datatype_size(const struct rte_dt_t *dt);
/* int rte_pmi_unpack (rte_iovec_t *data, void *src, uint32_t *offset); */
#endif

