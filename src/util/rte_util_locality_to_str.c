/*
 * Copyright (c) 2013       UT-Battelle, LLC. All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "rte.h"
#include "rte_util_locality.h"

const char const * rte_locality_strings[] = {
    "RTE_PROC_LOCALITY_UNKNOWN",
    "RTE_PROC_NON_LOCAL",
    "RTE_PROC_ON_CLUSTER",
    "RTE_PROC_ON_CU",
    "RTE_PROC_ON_NODE",
    "RTE_PROC_ON_BOARD",
    "RTE_PROC_ON_NUMA",
    "RTE_PROC_ON_SOCKET",
    "RTE_PROC_ON_L3CACHE",
    "RTE_PROC_ON_L2CACHE",
    "RTE_PROC_ON_L1CACHE",
    "RTE_PROC_ON_CORE",
    "RTE_PROC_ON_HWTHREAD",
    "RTE_PROC_ALL_LOCAL"
};

const char const * rte_locality_to_str (int locality)
{
    if (locality == RTE_PROC_LOCALITY_UNKNOWN)
        return rte_locality_strings[0];
    if (locality == RTE_PROC_NON_LOCAL)
        return rte_locality_strings[1];
    if (locality == RTE_PROC_ON_CLUSTER)
        return rte_locality_strings[2];
    if (locality == RTE_PROC_ON_CU)
        return rte_locality_strings[3];
    if (locality == RTE_PROC_ON_NODE)
        return rte_locality_strings[4];
    if (locality == RTE_PROC_ON_BOARD)
        return rte_locality_strings[5];
    if (locality == RTE_PROC_ON_NUMA)
        return rte_locality_strings[6];
    if (locality == RTE_PROC_ON_SOCKET)
        return rte_locality_strings[7];
    if (locality == RTE_PROC_ON_L3CACHE)
        return rte_locality_strings[8];
    if (locality == RTE_PROC_ON_L2CACHE)
        return rte_locality_strings[9];
    if (locality == RTE_PROC_ON_L1CACHE)
        return rte_locality_strings[10];
    if (locality == RTE_PROC_ON_CORE)
        return rte_locality_strings[11];
    if (locality == RTE_PROC_ON_HWTHREAD)
        return rte_locality_strings[12];
    if (locality == RTE_PROC_ALL_LOCAL)
        return rte_locality_strings[13];

    return rte_locality_strings[0];
}
