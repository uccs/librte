/*
 * Copyright (c) 2013       Oak Rigde National Laboratory
 *                          All rights reserved.
 *
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#ifndef RTE_PMI_H
#define RTE_PMI_H

#include "rte.h"

int rte_pmi_init(int *argc, char ***argv, rte_group_t *out_group);

int rte_pmi_finalize(void);

void rte_pmi_abort(int error_code, char *exit_description, ...);

rte_ec_handle_t rte_pmi_get_my_ec(void);

rte_ec_handle_t rte_pmi_group_index_to_ec(rte_group_t group,
                                          rte_node_index_t index);

int rte_pmi_group_size(rte_group_t group);

int rte_pmi_group_rank(rte_group_t group);

int rte_pmi_get_ec_locality(rte_ec_handle_t ec_handle);

int rte_pmi_cmp_ec(rte_ec_handle_t ec_handle_one, 
                   rte_ec_handle_t ec_handle_two);

rte_node_index_t rte_pmi_get_ec_index(rte_group_t group,
                                      rte_ec_handle_t ec_handle);

char * rte_pmi_get_ec_node_name(rte_ec_handle_t ec_handle);

char * rte_pmi_get_ec_hostname(rte_ec_handle_t ec_handle);

char * rte_pmi_get_session_dir(rte_ec_handle_t ec_handle);

int rte_pmi_cancel(rte_ec_handle_t  peer, int tag);

int rte_pmi_unpack(rte_iovec_t *data,
                   void *src,
                   uint32_t *offset);

int rte_pmi_recv(rte_iovec_t *iov,
                 uint32_t cnt,
                 rte_ec_handle_t src,
                 uint32_t tag,
                 rte_group_t group);

int rte_pmi_recv_nbcb(rte_ec_handle_t src,
                      uint32_t tag,
                      int flags,
                      rte_group_t group,
                      rte_request_cb_t cb_fn,
                      void * cb_data);

int rte_pmi_send(rte_iovec_t *iov,
                 uint32_t cnt,
                 rte_ec_handle_t dst,
                 uint32_t tag,
                 rte_group_t group);

int rte_pmi_send_nbcb(rte_iovec_t *iov,
                      uint32_t cnt,
                      rte_ec_handle_t dst,
                      uint32_t tag,
                      int flags,
                      rte_group_t group,
                      rte_request_cb_t cb_fn,
                      void * cb_data);

int rte_pmi_barrier(rte_group_t group);

int rte_pmi_srs_session_create(rte_group_t group,
                               int tag,
                               rte_srs_session_t *session);

int rte_pmi_srs_session_destroy(rte_srs_session_t session);

int rte_pmi_srs_get_data(rte_srs_session_t session,
                         rte_ec_handle_t peer,
                         rte_srs_key_t key,
                         void **value,
                         int *size);

int rte_pmi_srs_set_data(rte_srs_session_t session,
                         rte_srs_key_t key,
                         rte_iovec_t *value,
                         int elems);

int rte_pmi_srs_exchange_data(rte_srs_session_t session);

void rte_pmi_progress(void);

#endif /* RTE_PMI_H */
