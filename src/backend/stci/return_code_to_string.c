/*
 * Copyright (c)           2011 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include "stci/communication/p2p.h" /* required by rte.h */
#include "rte.h"

char * rte_stci_return_code_to_string(int code)
{
    switch(code) {
        default:
            return "rte_return_code_to_string is not implemented, sorry...";
    }

    return "Error, the function should not arrive to this flow";
}
