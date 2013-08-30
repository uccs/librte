/*
 * Copyright (c)      2013 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "rte.h"
#include "pmi.h"

RTE_PUBLIC void rte_pmi_abort(int error_code, char *exit_description, ...)
{
    int rc;

    va_list arglist;
    char* buffer = NULL;

    /* If there was a message, output it */
    va_start(arglist, exit_description);
    if( NULL != exit_description ) {
        vasprintf( &buffer, exit_description, arglist );
    }

    rc = PMI_Abort (error_code, buffer);
    if (PMI_SUCCESS == rc) {
        free( buffer );
        return;
    }

    /* we might want to put some warning here */

    free( buffer );
    return;
}

