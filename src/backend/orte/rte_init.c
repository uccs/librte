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
#include "orte_config.h"

#include "opal/runtime/opal.h"
#include "opal/util/arch.h"
#include "opal/mca/base/base.h"   
#include "opal/constants.h"

#include "orte/util/proc_info.h"
#include "orte/runtime/runtime.h"
#include "orte/runtime/orte_globals.h"

#include "rte.h"
#include "rte_internal.h"

volatile int MPIR_debug_gate = 1;
bool rte_initialized = false;

static void hook_debugger(void)
{
    while (MPIR_debug_gate == 0) {
#if defined(HAVE_USLEEP)
        usleep(100000); /* microseconds */
#else
        sleep(1);       /* seconds */
#endif
    }
}

RTE_PUBLIC int rte_orte_init(int *argc, char ***argv, rte_group_t *out_group)
{
    int rc;

    if (rte_initialized) 
        goto exit;

    rte_initialized = true;

    if (OPAL_SUCCESS != (rc = opal_init_util(NULL, NULL))) {
        return RTE_ERROR;
    }

    /* Pasha: This is MPI specific code 
    if (OPAL_SUCCESS != (ret = opal_arch_set_fortran_logical_size(sizeof(ompi_fortran_logical_t)))) {
        return RTE_ERROR;
    }
    */

    /* _After_ opal_init_util() but _before_ orte_init(), we need to
       set an MCA param that tells libevent that it's ok to use any
       mechanism in libevent that is available on this platform (e.g.,
       epoll and friends).  Per opal/event/event.s, we default to
       select/poll -- but we know that MPI processes won't be using
       pty's with the event engine, so it's ok to relax this
       constraint and let any fd-monitoring mechanism be used. */

    rc = mca_base_var_find("opal", "event", "*", "event_include");
    if (rc >= 0) {
        char *allvalue = "all";
        /* We have to explicitly "set" the MCA param value here
           because libevent initialization will re-register the MCA
           param and therefore override the default. Setting the value
           here puts the desired value ("all") in different storage
           that is not overwritten if/when the MCA param is
           re-registered. This is unless the user has specified a different
           value for this MCA parameter. Make sure we check to see if the
           default is specified before forcing "all" in case that is not what
           the user desires. Note that we do *NOT* set this value as an
           environment variable, just so that it won't be inherited by
           any spawned processes and potentially cause unintented
           side-effects with launching RTE tools... */
        mca_base_var_set_value(rc, allvalue, 4, MCA_BASE_VAR_SOURCE_DEFAULT, NULL);
    }

    opal_set_using_threads(false); /* true - just doesn't work */

    /* Pasha: Let's present ourself as ompi proc */
    rc = orte_init(NULL, NULL, ORTE_PROC_MPI);
    if (OPAL_SUCCESS != rc) {
        *out_group = NULL;
        return RTE_ERROR;
    }

    /* Temporary disabled, since it conflicts with OMPI and we don't really use 
       it with ddt so far.
    hook_debugger();
    */

    rc = all_proc_init();
    if (RTE_SUCCESS != rc) {
        return RTE_ERROR;
    }

exit:
    /* Pasha: Orte has no support for groups, instead we return job object */
    *out_group = &orte_process_info;
    
    return RTE_SUCCESS;
}

