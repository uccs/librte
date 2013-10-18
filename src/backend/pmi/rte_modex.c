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
#include "rte_dt.h"
#include "rte_pmi_internal.h"

#include <pmi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int rc, max_length;
    rte_pmi_srs_session_ptr_t _session = NULL;
    
    rc = PMI_KVS_Get_name_length_max (&max_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    _session = malloc ( sizeof(rte_pmi_srs_session_t));
    if (NULL == _session)
        return RTE_ERROR_OUT_OF_RESOURCE;
    
    _session->name = NULL;
    _session->name = malloc (max_length);
    if (NULL == _session->name)
        return RTE_ERROR_OUT_OF_RESOURCE;
 
    /* for now we get the "local" KVS session*/
    rc = PMI_KVS_Get_my_name (_session->name, max_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    *session = _session;
    
    return RTE_SUCCESS;
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
    /*int rc; */
    rte_pmi_srs_session_ptr_t _session;
    
    if (NULL == session)
        return RTE_ERROR_BAD_INPUT;
    
    _session = (rte_pmi_srs_session_ptr_t)session;

#if 0
    /* we do not destroy the session since we just support one at the moment */
    rc = PMI_KVS_Destroy (_session->name);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
#endif
    
    if (NULL != _session->name)
        free (_session->name);
    
    free (_session);
    
    return RTE_SUCCESS;
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
    int max_key_length, actual_key_length, max_value_length, rc, rank;
    size_t keysize;
    rte_pmi_srs_session_ptr_t _session;
    cray_pmi_proc_t *_ec_handle;
    char *value_buffer = NULL;
    char *_key;
    
    
    if (NULL == session)
        return RTE_ERROR_BAD_INPUT;
    
    _session = (rte_pmi_srs_session_ptr_t)session;
    _ec_handle = (cray_pmi_proc_t*)peer;
    
    rc = PMI_KVS_Get_value_length_max (&max_value_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    rc = PMI_KVS_Get_key_length_max (&max_key_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    /* get the key size */
    keysize = strlen (key);
    if (keysize > max_key_length)
        return RTE_ERROR; /* we might want a seperate error type here */
    
    /* get the rank */
    rank = _ec_handle - rte_pmi_procs;
    
    _key = malloc(max_key_length);
    actual_key_length = sprintf (_key, "%s_%d", key, rank);
    
    value_buffer = malloc (max_value_length);
    if (NULL == value_buffer)
        return RTE_ERROR_OUT_OF_RESOURCE;
    
    rc = PMI_KVS_Get (_session->name, _key, value_buffer, max_value_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    /* how do I get the length? */
    
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
RTE_PUBLIC int rte_pmi_srs_set_data (rte_srs_session_t session,
                                     rte_srs_key_t key,
                                     rte_iovec_t *iov,
                                     int iovcnt)
{
    int max_key_length, actual_key_length, max_value_length, rc, i, rank;
    size_t datasize, keysize;
    rte_pmi_srs_session_ptr_t _session;
    char *value_buffer = NULL;
    char *_key;
    
    if (NULL == session)
        return RTE_ERROR_BAD_INPUT;
    
    _session = (rte_pmi_srs_session_ptr_t)session;

    rc = PMI_KVS_Get_value_length_max (&max_value_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;

    rc = PMI_KVS_Get_key_length_max (&max_key_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;

    /* get the key size */
    keysize = strlen (key);
    if (keysize > max_key_length)
        return RTE_ERROR; /* we might want a seperate error type here */
    
    /* calculate the size of the data */
    datasize = get_datatype_size(iov->type) * iovcnt;
    if (datasize > max_value_length)
        return RTE_ERROR; /* we might want a seperate error type here */

    value_buffer = malloc (datasize);
    if (NULL == value_buffer)
        return RTE_ERROR_OUT_OF_RESOURCE;
    
    /* get the rank */
    rc = PMI_Get_rank (&rank);
    if (PMI_SUCCESS != rank)
        return RTE_ERROR;

    _key = malloc(max_key_length);
    actual_key_length = sprintf (_key, "%s_%d", key, rank);
    
    if (actual_key_length > max_key_length)
        return RTE_ERROR; /* TODO: add cleanup */

    /* by "hand" for now, we might want to put this in a seperate function */
    for (i=0; i < iovcnt; i++) {
        /* no packing here in the moment */
        memcpy(value_buffer + (i*get_datatype_size(iov->type)),
               iov[i].iov_base, get_datatype_size(iov->type));
    }

    rc = PMI_KVS_Put (_session->name, _key, value_buffer);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;

    /* I hope the buffer can be freed here */
    free (value_buffer);

    return RTE_SUCCESS;
}

RTE_PUBLIC int rte_pmi_srs_exchange_data(rte_srs_session_t session)
{
    int rc;
    
    rte_pmi_srs_session_ptr_t _session = (rte_pmi_srs_session_ptr_t)session;
    
    rc = PMI_KVS_Commit (_session->name);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    return RTE_SUCCESS;
}

