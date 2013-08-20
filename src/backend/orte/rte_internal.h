#ifndef RTE_INTERNAL_H
#define RTE_INTERNAL_H

#include "orte_config.h"

#include "opal/class/opal_list.h"
#include "orte/types.h"
#include "opal/mca/hwloc/hwloc.h"

struct rte_proc_t {
    opal_object_t super;
    struct orte_process_name_t name;
    opal_hwloc_locality_t locality;
    char* hostname;
};

typedef struct rte_proc_t rte_proc_t;
OBJ_CLASS_DECLARATION(rte_proc_t);

extern opal_pointer_array_t rte_procs;

int all_orte_proc_init(void);
int all_orte_proc_release(void);
#endif
