/*
 * Copyright (c) 2012-2013 Los Alamos National Security, LLC.  All rights
 *                         reserved.
 * Copyright (c) 2013-2014 UT-Battelle, LLC. All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include <pmi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#include "rte.h"
#include "rte_dt.h"
#include "rte_pmi_internal.h"

static char *pmi_encode(const void *val, size_t vallen);
static uint8_t* pmi_decode(const char *data, size_t *retlen);


/* Local variables */
bool rte_pmi_srs_initialized = false;

static char *pmi_kvs_name = NULL;
static int pmi_vallen_max = -1;
static int pmi_keylen_max = -1;

static int pmi_pack_key = 0;

static int rte_pmi_srs_setup_pmi(void)
{
    int max_length, rc;

#if RTE_WANT_PMI2
    pmi_vallen_max = PMI2_MAX_VALLEN;
    max_length = PMI2_MAX_VALLEN;
#else
    rc = PMI_KVS_Get_value_length_max(&pmi_vallen_max);
    if (PMI_SUCCESS != rc) {
        OPAL_OUTPUT_VERBOSE((1, opal_db_base_framework.framework_output,
                             "db:pmi:pmi_setup failed %s with error %s",
                             "PMI_Get_value_length_max",
                             opal_errmgr_base_pmi_error(rc)));
        return RTE_ERROR;
    }

    if (PMI_SUCCESS != (rc = PMI_KVS_Get_name_length_max(&max_length))) {
        OPAL_OUTPUT_VERBOSE((1, opal_db_base_framework.framework_output,
                             "db:pmi:pmi_setup failed %s with error %s",
                             "PMI_KVS_Get_name_length_max",
                             opal_errmgr_base_pmi_error(rc)));
        return RTE_ERROR;
    }
#endif
    pmi_kvs_name = (char*)malloc(max_length);
    if (NULL == pmi_kvs_name) {
        return RTE_ERROR_OUT_OF_RESOURCE;
    }

#if RTE_WANT_PMI2
    rc = PMI2_Job_GetId(pmi_kvs_name, max_length);
#else
    rc = PMI_KVS_Get_my_name(pmi_kvs_name,max_length);
#endif
    if (PMI_SUCCESS != rc) {
        fprintf(stderr, "db:pmi:pmi_setup failed %s with error %d on maxlength %d",
                        "PMI_KVS_Get_my_name", rc, max_length);
        return RTE_ERROR;
    }

#if RTE_WANT_PMI2
    pmi_keylen_max = PMI2_MAX_KEYLEN;
#else
    if (PMI_SUCCESS != (rc = PMI_KVS_Get_key_length_max(&pmi_keylen_max))) {
        fprintf(stderr, "db:pmi:pmi_setup failed %s with error %d",
                        "PMI_KVS_Get_key_length_max", rc);
        return RTE_ERROR;
    }
#endif

    return RTE_SUCCESS;
}

static char* setup_key(int session_no, rte_node_index_t idx, const char *key)
{
    char *pmi_kvs_key;

    if (pmi_keylen_max <= asprintf(&pmi_kvs_key, "%d-%" PRIu32 "-%s",
                                   session_no, idx, key)) {
        free(pmi_kvs_key);
        return NULL;
    }

    return pmi_kvs_key;
}

/* base64 encoding with illegal (to Cray PMI) characters removed ('=' is replaced by ' ') */
static inline unsigned char pmi_base64_encsym (unsigned char value) {
    assert (value < 64);

    if (value < 26) {
        return 'A' + value;
    } else if (value < 52) {
        return 'a' + (value - 26);
    } else if (value < 62) {
        return '0' + (value - 52);
    }

    return (62 == value) ? '+' : '/';
}

static inline unsigned char pmi_base64_decsym (unsigned char value) {
    if ('+' == value) {
        return 62;
    } else if ('/' == value) {
        return 63;
    } else if (' ' == value) {
        return 64;
    } else if (value <= '9') {
        return (value - '0') + 52;
    } else if (value <= 'Z') {
        return (value - 'A');
    } else if (value <= 'z') {
        return (value - 'a') + 26;
    }

    return 64;
}

static inline void pmi_base64_encode_block (const unsigned char in[3], char out[4], int len) {
    out[0] = pmi_base64_encsym (in[0] >> 2);
    out[1] = pmi_base64_encsym (((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4));
    /* Cray PMI doesn't allow = in PMI attributes so pad with spaces */
    out[2] = 1 < len ? pmi_base64_encsym(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) : ' ';
    out[3] = 2 < len ? pmi_base64_encsym(in[2] & 0x3f) : ' ';
}

static inline int pmi_base64_decode_block (const char in[4], unsigned char out[3]) {
    char in_dec[4];

    in_dec[0] = pmi_base64_decsym (in[0]);
    in_dec[1] = pmi_base64_decsym (in[1]);
    in_dec[2] = pmi_base64_decsym (in[2]);
    in_dec[3] = pmi_base64_decsym (in[3]);

    out[0] = in_dec[0] << 2 | in_dec[1] >> 4;
    if (64 == in_dec[2]) {
        return 1;
    }

    out[1] = in_dec[1] << 4 | in_dec[2] >> 2;
    if (64 == in_dec[3]) {
        return 2;
    }

    out[2] = ((in_dec[2] << 6) & 0xc0) | in_dec[3];
    return 3;
}

/* PMI only supports strings. For now, do a simple base64. */
static char *pmi_encode(const void *val, size_t vallen) {
    char *outdata, *tmp;
    size_t i;

    outdata = calloc (((2 + vallen) * 4) / 3 + 2, 1);
    if (NULL == outdata) {
        return NULL;
    }

    for (i = 0, tmp = outdata ; i < vallen ; i += 3, tmp += 4) {
        pmi_base64_encode_block((unsigned char *) val + i, tmp, vallen - i);
    }

    /* mark the end of the pmi string */
    tmp[0] = (unsigned char)'-';
    tmp[1] = (unsigned char)'\0';

    return outdata;
}

static uint8_t *pmi_decode (const char *data, size_t *retlen) {
    size_t input_len = (strlen (data) - 1) / 4;
    unsigned char *ret;
    int out_len;
    size_t i;

    /* default */
    *retlen = 0;

    ret = calloc (1, 3 * input_len + 1);
    if (NULL == ret) {
        return ret;
    }

    for (i = 0, out_len = 0 ; i < input_len ; i++, data += 4) {
        out_len += pmi_base64_decode_block(data, ret + 3 * i);
    }

    ret[out_len] = '\0';
    *retlen = out_len;
    return ret;
}

static int pmi_store_encoded(rte_pmi_srs_session_ptr_t session, const char *key, const rte_iovec_t *data, int elems)
{
    int i;
    size_t data_len = 0;
    size_t needed;

    int elem_size[elems];

    for (i=0;i<elems;i++) {
        switch (data[i].type->type) {
        case rte_pmi_bool:
        case rte_pmi_int8:
        case rte_pmi_uint8:
            elem_size[i] = data[i].count * get_datatype_size(data[i].type);
            data_len += elem_size[i];
            break;
        case rte_pmi_int16:
        case rte_pmi_uint16:
            elem_size[i] = data[i].count * get_datatype_size(data[i].type);
            data_len += elem_size[i];
            break;
        case rte_pmi_int32:
        case rte_pmi_uint32:
            elem_size[i] = data[i].count * get_datatype_size(data[i].type);
            data_len += elem_size[i];
            break;
        case rte_pmi_int64:
        case rte_pmi_uint64:
            elem_size[i]= data[i].count * get_datatype_size(data[i].type);
            data_len += elem_size[i];
            break;
        case rte_pmi_float2:
            elem_size[i]= data[i].count * get_datatype_size(data[i].type);
            data_len += elem_size[i];
            break;
        }
    }
    needed = strlen (key) + 6 + (elems*8) + data_len;

    if (NULL == session->pmi_packed_data) {
        session->pmi_packed_data = calloc (needed, 1);
    } else {
        /* grow the region */
        session->pmi_packed_data = realloc (session->pmi_packed_data, session->pmi_packed_data_offset + needed);
    }

    /* serialize the type and the count of the rte_iovec_t elements */
    session->pmi_packed_data_offset += sprintf (session->pmi_packed_data + session->pmi_packed_data_offset,
                                    "%s%c%04x%c", key, '\0', elems, '\0');
    for (i=0;i<elems;i++) {
	session->pmi_packed_data_offset += sprintf (session->pmi_packed_data + session->pmi_packed_data_offset,
                                                    "%02x%c%04x%c", data[i].type->type,'\0', (int) data[i].count, '\0');
    }

    /* finaly, add the data to the buffer */
    for(i=0;i<elems;i++) {
        if (NULL != data[i].iov_base) {
            memmove (session->pmi_packed_data + session->pmi_packed_data_offset, data[i].iov_base, elem_size[i]);
            session->pmi_packed_data_offset += elem_size[i];
        }
    }

    return RTE_SUCCESS;
}

/* Because Cray uses PMI2 extensions for some, but not all,
 * PMI functions, we define a set of wrappers for those
 * common functions we will use
 */
static int kvs_put(const char *key, const char *value)
{
#if RTE_WANT_PMI2
    return PMI2_KVS_Put(key, value);
#else
    return PMI_KVS_Put(pmi_kvs_name, key, value);
#endif
}

static int kvs_get(const char *key, char *value, int valuelen)
{
#if RTE_WANT_PMI2
    int len;

    return PMI2_KVS_Get(pmi_kvs_name, PMI2_ID_NULL, key, value, valuelen, &len);
#else
    return PMI_KVS_Get(pmi_kvs_name, key, value, valuelen);
#endif
}

static int pmi_commit_packed (rte_pmi_srs_session_ptr_t session) {
    char *pmikey = NULL, *tmp;
    char tmp_key[32], save;
    char *encoded_data;
    int rc, left, rank, i;
    librte_pmi_proc_t *_ec_handle;

    if (NULL == session)
        return RTE_ERROR_BAD_INPUT;

    if (session->pmi_packed_data_offset == 0) {
        /* nothing to write */
        return RTE_SUCCESS;
    }

    if (NULL == (encoded_data = pmi_encode(session->pmi_packed_data,
                                           session->pmi_packed_data_offset))) {
        return RTE_ERROR_OUT_OF_RESOURCE;
    }
    _ec_handle = rte_get_my_ec();

    /* get the rank */
    rank = _ec_handle - librte_pmi_procs;

    for (left = strlen (encoded_data), tmp = encoded_data, i = 0 ; left ; ) {
        size_t value_size = pmi_vallen_max > left ? left : pmi_vallen_max - 1;

        sprintf (tmp_key, "key%d", i);
        session->session_no = pmi_pack_key;

        if (NULL == (pmikey = setup_key(session->session_no, rank, tmp_key))) {
            rc = RTE_ERROR_BAD_INPUT;
            break;
        }

        /* only write value_size bytes */
        save = tmp[value_size];
        tmp[value_size] = '\0';

        rc = kvs_put(pmikey, tmp);
        free (pmikey);
        if (PMI_SUCCESS != rc) {
            rc = RTE_ERROR;
            fprintf(stderr, "Error commiting key %s\n", pmikey);
            break;
        }

        tmp[value_size] = save;
        tmp += value_size;
        left -= value_size;

        rc = RTE_SUCCESS;
    }
    if (encoded_data) {
        free (encoded_data);
    }

    session->pmi_packed_data_offset = 0;
    free (session->pmi_packed_data);
    session->pmi_packed_data = NULL;

    /* increase the session no */
    pmi_pack_key++;

    return rc;
}

static int
pmi_get_packed (const rte_node_index_t idx, rte_srs_session_t session,
                char **packed_data, size_t *len)
{
    char *tmp_encoded = NULL, *pmikey, *pmi_tmp;
    int remote_key = 0, size;
    size_t bytes_read;
    int rc;

    rte_pmi_srs_session_ptr_t _session = (rte_pmi_srs_session_ptr_t)session;

    char *pmi_key;
    /* set default */
    *packed_data = NULL;
    *len = 0;

    pmi_tmp = calloc (pmi_vallen_max, 1);
    if (NULL == pmi_tmp) {
        return RTE_ERROR_OUT_OF_RESOURCE;
    }

    /* read all of the packed data from this proc */
    char tmp_key[32];

    for (remote_key = 0, bytes_read = 0 ; ; ++remote_key) {

        sprintf (tmp_key, "key%d", remote_key);
        if (NULL == (pmikey = setup_key(_session->session_no, idx, tmp_key))) {
            rc = RTE_ERROR_OUT_OF_RESOURCE;
            return rc;
        }

        rc = kvs_get(pmikey, pmi_tmp, pmi_vallen_max);
        free (pmikey);
        if (PMI_SUCCESS != rc) {
            fprintf(stderr, "error fetching data\n");
        }
        size = strlen (pmi_tmp);

        if (NULL == tmp_encoded) {
            tmp_encoded = malloc (size + 1);
        } else {
            tmp_encoded = realloc (tmp_encoded, bytes_read + size + 1);
        }

        strcpy (tmp_encoded + bytes_read, pmi_tmp);
        bytes_read += size;

       /* is the string terminator present? */
        if ('-' == tmp_encoded[bytes_read-1]) {
            break;
        }
    }

    free (pmi_tmp);

    if (NULL != tmp_encoded) {
        *packed_data = (char *) pmi_decode (tmp_encoded, len);
        free (tmp_encoded);
        if (NULL == *packed_data) {
            return RTE_ERROR_OUT_OF_RESOURCE;
        }
    }

    return RTE_SUCCESS;
}
/*
 * libRTE function implementations
 *
 *
 */

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
    int rc;
    int max_length;

    rte_pmi_srs_session_ptr_t _session = NULL;

    if (!rte_pmi_srs_initialized) {
    	/* on the first call to session create we set up a few things for pmi */
        rte_pmi_srs_setup_pmi();
    }    
    rc = PMI_KVS_Get_name_length_max (&max_length);
    if (PMI_SUCCESS != rc)
        return RTE_ERROR;
    
    _session = malloc ( sizeof(rte_pmi_srs_session_t));
    if (NULL == _session)
        return RTE_ERROR_OUT_OF_RESOURCE;
    
    _session->name = NULL;
    _session->name = strdup(pmi_kvs_name);
    _session->pmi_packed_data = NULL;
    _session->pmi_packed_data_offset = 0;

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
    
    if (NULL != _session->name) {
        free (_session->name);
        _session->name = NULL;
    }        

    if (NULL != _session->pmi_packed_data) {
        free(_session->pmi_packed_data);
        _session->pmi_packed_data = NULL;
        _session->pmi_packed_data_offset = 0;
    }
     
    free (_session);
    _session = NULL;

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
    int rc, val_size;
    rte_pmi_srs_session_ptr_t _session;
    librte_pmi_proc_t *_ec_handle;
    char *value_buffer = NULL;
    size_t val_buf_length, offset;
    char *_key, *_type, *_elems, *_elem_header, *_data;
    rte_node_index_t rank;
    int num_elems, elem_idx;
    int data_len=0;
    
    if (NULL == session) {
        printf ("session in NULL");
        return RTE_ERROR_BAD_INPUT;
    }

    if (NULL == peer) return RTE_ERROR_BAD_INPUT;

    _session = (rte_pmi_srs_session_ptr_t)session;
    _ec_handle = (librte_pmi_proc_t*)peer;
    /* get the rank */
    rank = _ec_handle - librte_pmi_procs;

    pmi_get_packed (rank, session, &value_buffer, &val_buf_length);

    for (offset = 0 ; offset < val_buf_length && '\0' != value_buffer[offset] ; ) {
    /* find the key in the stream */
	_key = value_buffer + offset;
	/* type is encoded as 2 bytes */
        _elems = _key + strlen(_key) + 1;
        num_elems = strtol (_elems, NULL, 16);
        _elem_header = _elems + strlen(_elems) + 1;
        _data = _elem_header + 8*num_elems;

        for (elem_idx=0; elem_idx<num_elems;elem_idx++) {
            rte_pmi_dt_type_t type;
            int data_count;

            type = strtol (_elem_header + elem_idx * 8, NULL, 16);
            data_count = strtol (_elem_header + (elem_idx*8) + 3, NULL, 16);

            switch (type) {
                case rte_pmi_bool:
                case rte_pmi_int8:
                case rte_pmi_uint8:
                    data_len += data_count * 1;
                    break;
                case rte_pmi_int16:
                case rte_pmi_uint16:
                    data_len += data_count * 2;
                    break;
                case rte_pmi_int32:
                case rte_pmi_uint32:
                    data_len += data_count * 4;
                    break;
                case rte_pmi_int64:
                case rte_pmi_uint64:
                    data_len += data_count * 8;
                    break;
                case rte_pmi_float2:
                    data_len += data_count * 4;
                    break;
            }
	}

	if (0 == strcmp(_key, key)) {
            if (val_size == 0xffff) {
                *value = NULL;
                *size = 0;
            } else { 
                *value = malloc (data_len);
                memcpy(*value, _data, data_len);
                *size = data_len;
            }
            return RTE_SUCCESS;
        }
        
        offset = (size_t) (_data - value_buffer) + data_len;
	if (offset >= val_buf_length)
            return RTE_ERROR_NOT_FOUND;
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
RTE_PUBLIC int rte_pmi_srs_set_data (rte_srs_session_t session,
                                     rte_srs_key_t key,
                                     rte_iovec_t *iov,
                                     int iovcnt)
{
    return pmi_store_encoded(session, key, iov, iovcnt); 
}

RTE_PUBLIC int rte_pmi_srs_exchange_data(rte_srs_session_t session)
{
    pmi_commit_packed((rte_pmi_srs_session_ptr_t)session);    
    
#if RTE_WANT_PMI2
    PMI2_KVS_Fence();
#else
    {
        int rc;

        if (PMI_SUCCESS != (rc = PMI_KVS_Commit(pmi_kvs_name))) {
            OPAL_PMI_ERROR(rc, "PMI_KVS_Commit");
            return;
        }
        /* Barrier here to ensure all other procs have committed */
        PMI_Barrier();
    }
#endif
    return RTE_SUCCESS;
}

