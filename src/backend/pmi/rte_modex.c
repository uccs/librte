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

    printf ("session name: %s\n", _session->name); 

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
    int max_key_length, actual_key_length, maxvalue, rc, rank, vallen, max_value_length;
    size_t keysize;
    rte_pmi_srs_session_ptr_t _session;
    rte_pmi_proc_t *_ec_handle;
    char *value_buffer = NULL;
    char *_key;
    
    
    if (NULL == session) {
        printf ("session in NULL");
        return RTE_ERROR_BAD_INPUT;
    }
    
    _session = (rte_pmi_srs_session_ptr_t)session;
    _ec_handle = (rte_pmi_proc_t*)peer;
    
    rc = PMI_KVS_Get_value_length_max (&maxvalue);
    if (PMI_SUCCESS != rc) {
        fprintf (stderr, "PMI_KVS_Get_value_length_max failed (rc = %d)", rc);
        fflush (stderr);
        return RTE_ERROR;
    }
    
    rc = PMI_KVS_Get_key_length_max (&max_key_length);
    if (PMI_SUCCESS != rc) {
        fprintf (stderr, "PMI_KVS_Get_key_length_max failed (rc = %d)", rc);
        fflush (stderr);
        return RTE_ERROR;
    }
    
    /* get the key size */
    keysize = strlen (key);
    if (keysize > max_key_length) {
        fprintf (stderr, "key is to long");
        fflush (stderr);
        return RTE_ERROR; /* we might want a seperate error type here */
    }
    
    /* get the rank */
    rank = _ec_handle - rte_pmi_procs;
    
    _key = malloc(max_key_length);
	memset(_key, 0, max_key_length);
    actual_key_length = sprintf (_key, "%s_%d", key, rank);
	actual_key_length = strlen(_key);
    fprintf (stderr, "Fetching key %s\n", _key);
    
    value_buffer = malloc (maxvalue);
    if (NULL == value_buffer)
        return RTE_ERROR_OUT_OF_RESOURCE;

#if RTE_WANT_PMI2 == 0    
    rc = PMI_KVS_Get (_session->name, _key, value_buffer, max_value_length);
    if (PMI_SUCCESS != rc) {
        fprintf (stderr, "PMI_KVS_Get failed (rc = %d)", rc);
        fflush (stderr);
        return RTE_ERROR;
    }
#if HAVE_SLURM_PMI
    /* if we are running on top of slurm we need a barrier to actually publish the data */
    PMI_Barrier ();
#endif
#else
    rc = PMI2_KVS_Get(rte_pmi2_info.jobid, PMI2_ID_NULL, _key, value_buffer, PMI2_MAX_VALLEN, &vallen);
#endif

    *size = vallen;
    *value = value_buffer;
 
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
    int max_key_length, actual_key_length, max_value_length, rc, i, j, rank;
    size_t datasize = 0, keysize = 0;
    rte_pmi_srs_session_ptr_t _session;
    char *value_buffer = NULL;
    char *value_buffer_ptr = NULL;
    char *_key;
    
    if (NULL == session) {
        printf ("rte_pmi_srs_set_data: error -> session is NULL\n");
        return RTE_ERROR_BAD_INPUT;
    }
    
    _session = (rte_pmi_srs_session_ptr_t)session;

    rc = PMI_KVS_Get_value_length_max (&max_value_length);
    if (PMI_SUCCESS != rc) {
	printf ("rte_pmi_srs_set_data: error -> can not get max_value_length\n");
        return RTE_ERROR;
    }

    rc = PMI_KVS_Get_key_length_max (&max_key_length);
    if (PMI_SUCCESS != rc) {
        printf ("rte_pmi_srs_set_data: error -> can not get max_key_length\n");
        return RTE_ERROR;
    }

    /* get the key size */
    keysize = strlen (key);
    if (keysize > max_key_length) {
        printf ("rte_pmi_srs_set_data: error -> key to long\n");
        return RTE_ERROR; /* we might want a seperate error type here */
    }
    
    /* calculate the size of the data */
    for (i=0; i < iovcnt; i++) {
        datasize += get_datatype_size(iov[i].type) * iov[i].count;
    }

//   datasize += 1;

//    if (datasize > max_value_length) {
//        printf ("rte_pmi_srs_set_data: error -> datasize is to high (%d,%d)\n", datasize, max_value_length);
//        return RTE_ERROR; /* we might want a seperate error type here */
//    }

    value_buffer_ptr = value_buffer = malloc (max_value_length);
    if (NULL == value_buffer)
        return RTE_ERROR_OUT_OF_RESOURCE;
    
    /* get the rank */
    rc = PMI_Get_rank (&rank);
    if (PMI_SUCCESS != rc) {
        printf ("rte_pmi_srs_set_data: error -> can not get rank\n");
        return RTE_ERROR;
    }

    _key = malloc(max_key_length);
	memset(_key, 0, max_key_length);
    actual_key_length = sprintf (_key, "%s_%d", key, rank);
	actual_key_length = strlen(_key);
    
    if (actual_key_length > max_key_length) {
        printf ("rte_pmi_srs_set_data: error -> actual key to long\n");
        return RTE_ERROR; /* TODO: add cleanup */
    }

    /* by "hand" for now, we might want to put this in a seperate function */

    /* pmi treats the buffer as a strin -> we can not have 0 bytes in there
       or we will lose data
     */
    for (i=0; i < iovcnt; i++) {
        for (j=0; j < iov[i].count; j++) {
            int nb;
            switch (iov[i].type->type) {
                case rte_pmi_bool:
                case rte_pmi_int8:
                    nb = sprintf (value_buffer, "%d,", *(int8_t*)((iov[i].iov_base) + j * get_datatype_size(iov[i].type)));
                    value_buffer += nb;
                    break;
                case rte_pmi_uint8:
                    nb = sprintf (value_buffer, "%d,", (uint8_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb;
                    break;
                case rte_pmi_int16:
                    nb = sprintf (value_buffer, "%d", (int16_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_uint16:
                    nb = sprintf (value_buffer, "%d", (uint16_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_int32:
                    nb = sprintf (value_buffer, "%d", (int32_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_uint32:
                    nb = sprintf (value_buffer, "%d", (int32_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_int64:
                    nb = sprintf (value_buffer, "%ld", (int64_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_uint64:
                    nb = sprintf (value_buffer, "%ld", (uint64_t*)(iov[i].iov_base) + j * get_datatype_size(iov[i].type));
                    value_buffer += nb * get_datatype_size(iov[i].type);
                    break;
                case rte_pmi_float2:
                    break;
            }
        }
    }

    printf ("session name: %s, key: %s, value_buffer: %s\n", _session->name, _key, value_buffer);
#if RTE_WANT_PMI2 == 0
    rc = PMI_KVS_Put (_session->name, _key, value_buffer_ptr);
    if (PMI_SUCCESS != rc) {
        printf ("rte_pmi_srs_set_data: error -> PMI_KVS_Put failed (rc = %d, PMI_FAIL = %d)\n", rc, PMI_FAIL);
        return RTE_ERROR;
    }
#else
    rc = PMI2_KVS_Put (_key, value_buffer_ptr);
    if (PMI_SUCCESS != rc) {
        printf ("rte_pmi_srs_set_data: error -> PMI_KVS_Put failed (rc = %d, PMI_FAIL = %d)\n", rc, PMI_FAIL);
        return RTE_ERROR;
    }
#endif
    /* I hope the buffer can be freed here */
    free (value_buffer_ptr);

    return RTE_SUCCESS;
}

RTE_PUBLIC int rte_pmi_srs_exchange_data(rte_srs_session_t session)
{
    int rc;
#if RTE_WANT_PMI2 == 0
    rte_pmi_srs_session_ptr_t _session = (rte_pmi_srs_session_ptr_t)session;
    
    rc = PMI_KVS_Commit (_session->name);
#else
    rc = PMI2_KVS_Fence ();
#endif
    if (PMI_SUCCESS != rc) {
        fprintf (stderr, "rc = %d", rc);
        fflush (stderr);
        return RTE_ERROR;
    }
    
    return RTE_SUCCESS;
}

