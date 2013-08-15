/*
 * Copyright (c) 2012       Oak Ridge National Laboratory.
 *                          All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "rte.h"
#include <stdio.h>

int main (int argc, char **argv)
{
    int rc                         = 0;

    rte_group_t group_world;
    
    /* initialize the run tim environment */
    rc = rte_init (&argc, &argv, &group_world);
    
    printf ("sizeof rte_int1 is %lu\n", sizeof (rte_datatype_int8_t));
    printf ("sizeof rte_int2 is %lu\n", sizeof (rte_datatype_int16_t));
    printf ("sizeof rte_int4 is %lu\n", sizeof (rte_datatype_int32_t));
    printf ("sizeof rte_int8 is %lu\n", sizeof (rte_datatype_int64_t));

    printf ("sizeof rte_uint1 is %lu\n", sizeof (rte_datatype_uint8_t));
    printf ("sizeof rte_uint2 is %lu\n", sizeof (rte_datatype_uint16_t));
    printf ("sizeof rte_uint4 is %lu\n", sizeof (rte_datatype_uint32_t));
    printf ("sizeof rte_uint8 is %lu\n", sizeof (rte_datatype_uint64_t));

    printf ("sizeof rte_float2 is %lu\n", sizeof (rte_datatype_float_t));
    
    printf ("sizeof rte_bool is %lu\n", sizeof (rte_datatype_bool_t));
    
    /* shut down the run tim environment */
    rte_finalize ();
    
    return rc;
}

