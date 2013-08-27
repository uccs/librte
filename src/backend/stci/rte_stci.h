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

#ifndef RTE_STCI_H
#define RTE_STCI_H

#include "rte.h"

int rte_stci_init(int *argc, char ***argv, rte_group_t *out_group);

int rte_stci_finalize(void);

void rte_stci_abort(int error_code, char *exit_description, ...);

rte_ec_handle_t rte_stci_get_my_ec(void);

rte_ec_handle_t rte_stci_group_index_to_ec(rte_group_t group,
                                           rte_node_index_t index);

int rte_stci_group_size(rte_group_t group);

int rte_stci_group_rank(rte_group_t group);

int rte_stci_get_ec_locality(rte_ec_handle_t ec_handle);

int rte_stci_cmp_ec(rte_ec_handle_t ec_handle_one, 
                    rte_ec_handle_t ec_handle_two);

rte_node_index_t rte_stci_get_ec_index(rte_group_t group,
                                       rte_ec_handle_t ec_handle);

char * rte_stci_get_ec_node_name(rte_ec_handle_t ec_handle);

char * rte_stci_get_ec_hostname(rte_ec_handle_t ec_handle);

char * rte_stci_get_session_dir(rte_ec_handle_t ec_handle);

int rte_stci_cancel(rte_ec_handle_t  peer, int tag);

int rte_stci_unpack(rte_iovec_t *data,
                    void *src,
                    uint32_t *offset);

int rte_stci_recv(rte_iovec_t *iov,
                  uint32_t cnt,
                  rte_ec_handle_t src,
                  uint32_t tag,
                  rte_group_t group);

int rte_stci_recv_nbcb(rte_ec_handle_t src,
                       uint32_t tag,
                       int flags,
                       rte_group_t group,
                       rte_request_cb_t cb_fn,
                       void * cb_data);

int rte_stci_send(rte_iovec_t *iov,
                  uint32_t cnt,
                  rte_ec_handle_t dst,
                  uint32_t tag,
                  rte_group_t group);

int rte_stci_send_nbcb(rte_iovec_t *iov,
                       uint32_t cnt,
                       rte_ec_handle_t dst,
                       uint32_t tag,
                       int flags,
                       rte_group_t group,
                       rte_request_cb_t cb_fn,
                       void * cb_data);

int rte_stci_barrier(rte_group_t group);

int rte_stci_srs_session_create(rte_group_t group,
                                int tag,
                                rte_srs_session_t *session);

int rte_stci_srs_session_destroy(rte_srs_session_t session);

int rte_stci_srs_get_data(rte_srs_session_t session,
                          rte_ec_handle_t peer,
                          rte_srs_key_t key,
                          void **value,
                          int *size);

int rte_stci_srs_set_data(rte_srs_session_t session,
                          rte_srs_key_t key,
                          rte_iovec_t *value,
                          int elems);

int rte_stci_srs_exchange_data(rte_srs_session_t session);

void rte_stci_progress(void);

#endif /* RTE_STCI_H */
