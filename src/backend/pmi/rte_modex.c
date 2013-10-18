/*
 * Copyright (c) 2013       UT-Battelle, LLC. All rights reserved.
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
#include <pmi.h>

/**
 * @brief Create a srs session
 *
 * @param[in]     group    the group this session is limited to
 * @param[in]     tag      an arbitary tag
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_pmi_srs_session_create (rte_group_t group,
                                           int tag,
                                           rte_srs_session_t *session)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief destroy a previously created srs session
 *
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_pmi_srs_session_destroy (rte_srs_session_t session)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief blocking call to request for a key/value pair from a session
 *
 * @param[in]  session  NULL for global exchange, rte_srs_session_t for
 *                      scoped exchange
 * @param[in]  peer     the peer we want the data from
 * @param[in]  key      the key the value will be assigned to
 * @param[out] value    pointer to the data buffer for the given key
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_pmi_srs_get_data(rte_srs_session_t session,
                                    rte_ec_handle_t peer,
                                    rte_srs_key_t key,
                                    void **value,
                                    int *size)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
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
RTE_PUBLIC int rte_pmi_srs_set_data (rte_srs_session_t session,
                                     rte_srs_key_t key,
                                     rte_iovec_t *iov,
                                     int iovcnt)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

RTE_PUBLIC int rte_pmi_srs_exchange_data(rte_srs_session_t session)
{
    return RTE_ERROR_NOT_IMPLEMENTED;
}

