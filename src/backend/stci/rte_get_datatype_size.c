/*
 * Copyright (c) 2012       Oak Rigde National Laboratory
 *                          All rights reserved.
 *
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include "rte.h"
#include "rte_dt.h"

#include "opal/datatype/opal_datatype.h"

size_t rte_stci_get_datatype_size (rte_datatype_t dt) {
    size_t dt_size;
    
    opal_datatype_type_size (&(dt->opal_dt), &dt_size);
    
    return dt_size;
}
