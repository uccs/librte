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

#include "rte.h"
#include <pmi.h>

/** Process locality definitions */
enum {
    RTE_PMI_PROC_LOCALITY_UNKNOWN  = 0x0000,
    RTE_PMI_PROC_NON_LOCAL         = 0x8000,
    RTE_PMI_PROC_ON_CLUSTER        = 0x0400,
    RTE_PMI_PROC_ON_CU             = 0x0200,
    RTE_PMI_PROC_ON_NODE           = 0x0100,
    RTE_PMI_PROC_ON_BOARD          = 0x0080,
    RTE_PMI_PROC_ON_NUMA           = 0x0040,
    RTE_PMI_PROC_ON_SOCKET         = 0x0020,
    RTE_PMI_PROC_ON_L3CACHE        = 0x0010,
    RTE_PMI_PROC_ON_L2CACHE        = 0x0008,
    RTE_PMI_PROC_ON_L1CACHE        = 0x0004,
    RTE_PMI_PROC_ON_CORE           = 0x0002,
    RTE_PMI_PROC_ON_HWTHREAD       = 0x0001,
    RTE_PMI_PROC_ALL_LOCAL         = 0x0fff
};

const int RTE_PROC_LOCALITY_UNKNOWN = RTE_PMI_PROC_LOCALITY_UNKNOWN;
const int RTE_PROC_NON_LOCAL =        RTE_PMI_PROC_NON_LOCAL;
const int RTE_PROC_ON_CLUSTER =       RTE_PMI_PROC_ON_CLUSTER;
const int RTE_PROC_ON_CU =            RTE_PMI_PROC_ON_CU;
const int RTE_PROC_ON_NODE =          RTE_PMI_PROC_ON_NODE;
const int RTE_PROC_ON_BOARD =         RTE_PMI_PROC_ON_BOARD;
const int RTE_PROC_ON_NUMA =          RTE_PMI_PROC_ON_NUMA;
const int RTE_PROC_ON_SOCKET =        RTE_PMI_PROC_ON_SOCKET;
const int RTE_PROC_ON_L3CACHE =       RTE_PMI_PROC_ON_L3CACHE;
const int RTE_PROC_ON_L2CACHE =       RTE_PMI_PROC_ON_L2CACHE;
const int RTE_PROC_ON_L1CACHE =       RTE_PMI_PROC_ON_L1CACHE;
const int RTE_PROC_ON_CORE =          RTE_PMI_PROC_ON_CORE;
const int RTE_PROC_ON_HWTHREAD =      RTE_PMI_PROC_ON_HWTHREAD;
const int RTE_PROC_ALL_LOCAL =        RTE_PMI_PROC_ALL_LOCAL;

RTE_PUBLIC int rte_pmi_get_ec_locality (rte_ec_handle_t ec_handle)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

