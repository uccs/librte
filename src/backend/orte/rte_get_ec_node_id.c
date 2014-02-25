#include "rte.h"
#include "orte/runtime/orte_globals.h"

uint32_t rte_orte_get_ec_node_id (rte_ec_handle_t ec_context)
{
    return ORTE_PROC_MY_DAEMON->vpid;
}
