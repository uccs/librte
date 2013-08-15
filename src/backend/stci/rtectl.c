/*
 * Copyright (c) 2011-2012  Oak Rigde National Laboratory
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
#include "stci/runtime/stci_runtime.h"
#include "stci/runtime/stci_progress.h"
#include "stci/util/util_debug.h"


/**
 * Backend function for rtectl to get/set the jobid of a STCI agent
 * @param[in]   op  The type of operation to perform (get or set)
 * @param[in]   proc_name   Target STCI agent
 * @param       value       Pointer where to get/set the agent's jobid
 */
static inline int 
rtectl_stci_proc_jobid (rtectl_op_t op,
                        STCI_Process_name_t *proc_name,
                        int *value)
{
    switch (op) {
        case RTECTL_GET:
            *value = STCI_PROCESS_NAME_GET_JOBID (proc_name);
            break;
        case RTECTL_SET:
            STCI_PROCESS_NAME_SET_JOBID (proc_name, *value);
            break;
        default:
            STCI_FATAL (STCI_ERROR, ("Unsupported operation (%d)", op));
    }

    return STCI_SUCCESS;
}

/**
 * Backend function for rtectl to get/set the vpid of a STCI agent
 * @param[in]   op  The type of operation to perform (get or set)
 * @param[in]   proc_name   Target STCI agent
 * @param       value       Pointer where to get/set the agent's vpid
 */
static inline int 
rtectl_stci_proc_vpid (rtectl_op_t op,
                       STCI_Process_name_t *proc_name, 
                       int *value)
{
    switch (op) {
        case RTECTL_GET:
            *value = STCI_PROCESS_NAME_GET_VPID (proc_name);
            break;
        case RTECTL_SET:
            STCI_PROCESS_NAME_SET_VPID (proc_name, *value);
            break;
        default:
            STCI_FATAL (STCI_ERROR, ("Unsupported operation (%d)", op));
    }

    return STCI_SUCCESS;
}

/**
 * Backend function for rtectl to get/set the epoch of a STCI agent
 * @param[in]   op  The type of operation to perform (get or set)
 * @param[in]   proc_name   Target STCI agent
 * @param       value       Pointer where to get/set the agent's epoch
 */
static inline int 
rtectl_stci_proc_epoch (rtectl_op_t op,
                        STCI_Process_name_t *proc_name, 
                        int *value)
{
    switch (op) {
        case RTECTL_GET:
            *value = STCI_PROCESS_NAME_GET_VPIDEPOCH (proc_name);
            break;
        case RTECTL_SET:
            STCI_PROCESS_NAME_SET_VPIDEPOCH (proc_name, *value);
            break;
        default:
            STCI_FATAL (STCI_ERROR, ("Unsupported operation (%d)", op));
    }

    return STCI_SUCCESS;
}

/**
 * We currently assume that the state of the RTE is actually the state of the
 * RTE in the context of the current proc. In the context of STCI, this equals
 * the state of the agent.
 * ptr is not used in this case
 */
static inline int
rtectl_stci_state (rtectl_op_t op, void *ptr, rte_state_t *value)
{
    rte_state_t state = RTE_STATE_INITIATING;

    /* We can only query the state */
    if (op == RTECTL_GET) {
        if (stci_process_self->state == STCI_PROCESS_STATE_RUNNING)
            state = RTE_STATE_INITED;
        if (stci_process_self->state == STCI_PROCESS_STATE_FINISHED ||
            stci_process_self->state == STCI_PROCESS_STATE_HAS_TO_FINISH)
            state = RTE_STATE_FINALIZING;
        *value = state;
    } else {
        STCI_FATAL (STCI_ERROR, ("Unsupported operation (%d)", op));
    }

    return STCI_SUCCESS;  
}

/**
 * Just set the progress mode on the default stci event domain.
 */
static inline int
rtectl_stci_progress_mode (rtectl_op_t op, void *ptr, rte_progress_mode_t *value)
{
    stci_event_mode_t mode;
    
    /* We can only query the state */
    if (op == RTECTL_GET) {
        stci_event_get_mode (stci_event_domain, &mode);
        
        switch (mode) {
            case STCI_EVENT_NOBLOCK:
                *value = RTE_PROGRESS_NON_BLOCKING;
                break;
            case STCI_EVENT_BLOCK:
                *value = RTE_PROGRESS_BLOCKING;
                break;
        }
    } else {
        switch (*value) {
            case RTE_PROGRESS_NON_BLOCKING:
                mode = STCI_EVENT_NOBLOCK;
                break;
            case RTE_PROGRESS_BLOCKING:
                mode = STCI_EVENT_BLOCK;
                break;
        }
        
        stci_event_set_mode (stci_event_domain, mode);
    }
    
    return STCI_SUCCESS;
}

/**
 * Public function to get/set RTE parameter, such as processes' parameters.
 * This function is only a wrapper that redirect the call to the appropriate
 * STCI functions. For each pre-defined RTE key and each predefined operation,
 * we define the STCI equivalent.
 * Note that we decided to keep in a single function both the get and set 
 * operations in order to ease maintenance and code reading. 
 */
int rtectl (rtectl_op_t op, rtectl_key_t key, void *ptr, void *value) {
    switch (key) {
        case RTECTL_KEY_JOBID:
            rtectl_stci_proc_jobid (op, (STCI_Process_name_t*)ptr, (int*)value);
            break;
        case RTECTL_KEY_VPID:
            rtectl_stci_proc_vpid (op, (STCI_Process_name_t*)ptr, (int*)value);
            break;
        case RTECTL_KEY_EPOCH:
            rtectl_stci_proc_epoch (op, (STCI_Process_name_t*)ptr, (int*)value);
        case RTECTL_KEY_RTESTATE:
            rtectl_stci_state (op, ptr, (rte_state_t*)value);
        case RTECTL_KEY_PROGRESSMODE:
            rtectl_stci_progress_mode (op, ptr, (rte_progress_mode_t*)value);
        default:
            STCI_FATAL (STCI_ERROR, ("Invalid key %d", key));
    }

    return STCI_SUCCESS;
}
