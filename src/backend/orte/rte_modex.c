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
#include "orte_config.h"

#include "rte_dt.h"
#include "opal/dss/dss.h"
#include "opal/mca/db/db.h"
#include "orte/runtime/orte_globals.h"
#include "rte.h"
#include "orte/mca/rml/rml.h"
#include "orte/mca/grpcomm/grpcomm.h"

/* ORTE support only single modex session. */
bool modex_used = false;

/**
 * @brief Create a srs session
 *
 * @param[in]     group    the group this session is limited to
 * @param[in]     tag      an arbitary tag
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_orte_srs_session_create (rte_group_t group,
                                       int tag,
                                       rte_srs_session_t *session)
{
    /* ORTE doesn't support multile sessions.
     * As a result, we piggy back on existing session.
     * This is hacky solutiong for now.
     */
#if 0
    if (modex_used)
        return RTE_ERROR;
    else {
        modex_used = true;
        *session = (rte_srs_session_t *)&modex_used;
        return RTE_SUCCESS;
    }
#else 
    modex_used = true;
    *session = (rte_srs_session_t *)&modex_used;
    return RTE_SUCCESS;
#endif 
}

/**
 * @brief destroy a previously created srs session
 *
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_orte_srs_session_destroy (rte_srs_session_t session)
{
    return  modex_used ? RTE_SUCCESS : RTE_ERROR;
}

/**
 * @brief blocking call to request for a key/value pair from a session
 *
 * @param[in]  session  NULL for global exchange, rte_srs_session_t for
 *                      scoped exchange
 * @param[in]  peer     the peer we want the data from
 * @param[in]  key      the key the value will be assigned to
 * @param[out] value    bointer to the data buffer for the given key
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_orte_srs_get_data(rte_srs_session_t session,
                                rte_ec_handle_t peer,
                                rte_srs_key_t key,
                                void **value,
                                int *size)
{
    int rc;
    opal_byte_object_t *boptr = NULL;
    *value = NULL;
    *size = 0;

    if (!modex_used || &modex_used != session) {
        return RTE_ERROR;
    }

    if (NULL == key || NULL == peer) {
        return RTE_ERROR_BAD_INPUT;
    }

    /* the fetch API returns a pointer to the data */
    rc = opal_db.fetch((opal_identifier_t*)peer, key,(void**)&boptr, OPAL_BYTE_OBJECT);
    if (OPAL_SUCCESS == rc) {
        opal_buffer_t *buffer = OBJ_NEW(opal_buffer_t);;
        /* Load values */
        rc = opal_dss.load(buffer,
                            (void*)boptr->bytes, boptr->size);
        if (OPAL_SUCCESS != rc) {
            return RTE_ERROR;
        }
        *value = (void*)buffer;
        *size = boptr->size;
        free(boptr);
        rc = RTE_SUCCESS;
    } else {
        rc = RTE_ERROR;
    }
    return rc;
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
RTE_PUBLIC int rte_orte_srs_set_data (rte_srs_session_t session,
                                 rte_srs_key_t key,
                                 rte_iovec_t *iov,
                                 int iovcnt)
{
    int rc;
    int32_t vec_i;
    uint32_t c_i;
    size_t offset = 0;
    void *dataptr;
    opal_byte_object_t bo;
    orte_std_cntr_t datalen;
    opal_buffer_t* buffer;


    if (!modex_used || &modex_used != session){
        return RTE_ERROR;
     }

    if (NULL == key || 0 == iovcnt || NULL == iov) {
        return RTE_ERROR_BAD_INPUT;
    }

    buffer = OBJ_NEW(opal_buffer_t);
    /* Pack data */
    for (vec_i = 0; vec_i < iovcnt; ++vec_i) {
        offset = 0;
        for(c_i = 0; c_i < iov[vec_i].count; ++c_i) {
            rc = opal_dss.pack(buffer,
                    (void *)((char *)iov[vec_i].iov_base + offset),
                    1, (opal_data_type_t)iov[vec_i].type->opal_dt);
            if (OPAL_SUCCESS != rc) {
                rc = RTE_ERROR;
                goto out;
            }
            offset += get_datatype_size(iov[vec_i].type);
        }
    }

    /* Repack data */
    rc = opal_dss.unload(buffer, &dataptr, &datalen);
    if (rc != OPAL_SUCCESS) {
        rc = RTE_ERROR;
        goto out;
    }

    opal_dss.load(buffer, dataptr, datalen);
    bo.bytes = (uint8_t *)dataptr;
    bo.size = datalen;

    /* Post the data */
    rc = opal_db.store((opal_identifier_t*)rte_get_my_ec(), OPAL_DB_GLOBAL, key, (void *)&bo, OPAL_BYTE_OBJECT);
    if (OPAL_SUCCESS != rc) {
        rc = RTE_ERROR;
        goto out;
    }

    rc = RTE_SUCCESS;
out:
    free(buffer);
    return rc;
}

RTE_PUBLIC int rte_orte_srs_exchange_data(rte_srs_session_t session)
{
    int rc;
    orte_grpcomm_collective_t *coll;

    coll = OBJ_NEW (orte_grpcomm_collective_t);
    if (!modex_used || &modex_used != session) {
        return RTE_ERROR;
    }
    coll->id = orte_process_info.peer_modex;

    if (OPAL_SUCCESS != (rc = orte_grpcomm.modex(coll))) {
        return RTE_ERROR;
    }

    coll->active = true;

    while (coll->active) {
        opal_progress();
    }

    OBJ_RELEASE (coll);
    return RTE_SUCCESS;
}

