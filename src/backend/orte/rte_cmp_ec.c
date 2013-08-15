/*
 * Copyright (c) 2011      Oak Rigde National Laboratory
 *                         All rights reserved.
 */

#include "orte_config.h"

#include "orte/runtime/orte_globals.h"
#include "rte.h"

int rte_orte_cmp_ec (rte_ec_handle_t ec_handle_one, rte_ec_handle_t ec_handle_two)
{
    return orte_util_compare_name_fields(ORTE_NS_CMP_ALL,
                                      ec_handle_one,
                                      ec_handle_two);
}

