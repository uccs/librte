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
#include "stci/mca/modex/modex.h"

/**
 * @brief Create a srs session
 *
 * @param[in]     group    the group this session is limited to
 * @param[in]     tag      an arbitary tag
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_stci_srs_session_create   (rte_group_t        group  ,
                                         int                tag    ,
                                         rte_srs_session_t *session)
{
    int rc = RTE_SUCCESS;

    rc = stci_modex.session_create (group, tag, (stci_modex_session_t**)session);
    if (STCI_SUCCESS != rc) {
        opal_output (0, "Impossible to call stci_modex.session_create()");
        return RTE_ERROR;
    }
    
    return RTE_SUCCESS;
}

/**
 * @brief destroy a previously created srs session
 *
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_stci_srs_session_destroy (rte_srs_session_t session)
{
    int rc = RTE_SUCCESS;
    
    rc = stci_modex.session_destroy ((stci_modex_session_t*)session);
    if (STCI_SUCCESS != rc) {
        opal_output (0, "Impossible to call stci_modex.session_destroy()");
        return RTE_ERROR;
    }
    
    return rc;
}

/**
 * @brief blocking call to request for a key/value pair from a session
 *
 * @param[in]  session  NULL for global exchange, rte_srs_session_t for
 *                      scoped exchange
 * @param[in]  peer     the peer we want the data from
 * @param[in]  key      the key the value will be assigned to
 * @param[out] value    pointer to the data buffer for the given key
 * @param[out] size     size of the data in buffer for the given key
 *
 * @return RTE_SUCCESS on success, 
 *         RTE_ERROR_NOT_FOUND if no value found for key,
 *         RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_stci_srs_get_data (rte_srs_session_t   session,
                                 rte_ec_handle_t     peer   ,
                                 rte_srs_key_t       key    ,
                                 void              **value  ,
                                 int                *size   )
{
    int rc = RTE_SUCCESS;
    
    rc = stci_modex.get_agent_data (session, peer, key, value, size);
    
    if (STCI_SUCCESS != rc) {
        if (STCI_ERR_NOT_FOUND == rc) {
        opal_output (0, "Impossible to find key in modex\n");
            return RTE_ERROR_NOT_FOUND;
        }
        opal_output (0, "Impossible to call stci_modex.get_agent_data()\n");
        return RTE_ERROR;
    }
    
    return RTE_SUCCESS;
}

/**
 * @brief blocking call to register a key/value pair with a session
 *
 * @note If the key provided already is registred with the session 
 *       RTE_ERR_DUPLICATE_KEY is returned.
 *
 * @param[in]  session     NULL for global exchange, rte_srs_session_t for
 *                         scoped exchange
 * @param[in]  key         the key the value will be assigned to
 * @param[in]  value       the data stored in the srs
 * @param[in]  elems       number of elements to store in srs
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_stci_srs_set_data (rte_srs_session_t  session,
                                 rte_srs_key_t      key    ,
                                 rte_iovec_t       *iov    ,
                                 int                iovcnt )
{
    int rc = RTE_SUCCESS;
    
    rc = stci_modex.prepare_to_publish (session, key,
                                        (const struct stci_tlv_t *)iov, iovcnt);
    if (STCI_SUCCESS != rc) {
        opal_output (0, "Impossible to call stci_modex.prepare_to_publish()");
        return RTE_ERROR;
    }
    
    return RTE_SUCCESS;
}

RTE_PUBLIC int rte_stci_srs_exchange_data (rte_srs_session_t session)
{
    int rc = RTE_SUCCESS;
    
    STCI_Request_t *request     = NULL;
    
    rc = stci_modex.publish_data_nb (session, &request);
    if (STCI_SUCCESS != rc) {
        opal_output (0, "Impossible to call stci_modex.publish_data_nb()");
        return RTE_ERROR;
    }
    
    rc = stci_wait (request, NULL);
    if (rc != STCI_SUCCESS) {
        opal_output (0, "Impossible to call stci_wait ()");
        return RTE_ERROR;
    }
    return RTE_SUCCESS;
}
