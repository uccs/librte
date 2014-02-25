#include "rte.h"
#include "orte/util/proc_info.h"

uint32_t rte_orte_get_job_id ()
{
    return orte_process_info.my_name.jobid;
}
