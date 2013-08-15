/*
 * Copyright (c)      2013  Oak Rigde National Laboratory
 *                          All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#ifndef rte_rte_request_h
#define rte_rte_request_h

#include "rte.h"

typedef struct rte_request_data_s {
    rte_request_cb_t  cb_fn;
    void             *cb_data;
} rte_request_data_t;

#endif
