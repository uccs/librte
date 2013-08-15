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

#include "rte.h"
#include "opal/runtime/opal_progress.h"

/*
 * FIXME: we need an actual implementation for this function or come up with a
 *        better solution.
 * FIXME: Statuses are not handled in the moment.
 */

inline void rte_orte_progress (void) {
    if (!opal_using_threads()) 
        opal_progress();
}
