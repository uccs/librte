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

#include <rte.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "rte_config.h"

/* include the headers for the backends  */
#include "src/backend/orte/rte_orte.h"
#include "src/backend/stci/rte_stci.h"

/**
 * @file
 *
 * @author Swen Boehm
 */

/* types for the function pointers into the back-end implementation */

typedef int (*rte_be_init_fn_t)
            (int *argc, char ***argv, rte_group_t *out_group);

typedef int (*rte_be_finalize_fn_t)(void);

typedef void (*rte_be_abort_fn_t)(int error_code, char
            *exit_description, ...);

typedef rte_ec_handle_t (*rte_be_get_my_ec_fn_t)(void);

typedef rte_ec_handle_t (*rte_be_group_index_to_ec_fn_t)
                        (rte_group_t group, rte_node_index_t index);

typedef int (*rte_be_group_size_fn_t)(rte_group_t group);

typedef int (*rte_be_group_rank_fn_t)(rte_group_t group);

typedef int (*rte_be_get_ec_locality_fn_t)(rte_ec_handle_t ec_handle);

typedef int (*rte_be_cmp_ec_fn_t)
            (rte_ec_handle_t ec_handle_one,
             rte_ec_handle_t ec_handle_two);

typedef rte_node_index_t (*rte_be_get_ec_index_fn_t)
                         (rte_group_t group, rte_ec_handle_t ec_handle);

typedef char* (*rte_be_get_ec_node_name_fn_t)(rte_ec_handle_t ec_handle);

typedef char* (*rte_be_get_ec_hostname_fn_t)(rte_ec_handle_t ec_handle);

typedef char* (*rte_be_get_session_dir_fn_t)(rte_ec_handle_t ec_handle);

typedef int (*rte_be_cancel_fn_t)(rte_ec_handle_t peer, int tag);

typedef int (*rte_be_unpack_fn_t)
            (rte_iovec_t *data, void *src, uint32_t *offset);

typedef int (*rte_be_recv_fn_t)
            (rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t src,
             uint32_t tag, rte_group_t group);

typedef int (*rte_be_recv_nbcb_fn_t)
            (rte_ec_handle_t src, uint32_t tag, int flags, rte_group_t group,
             rte_request_cb_t cb_fn, void * cb_data);

typedef int (*rte_be_send_fn_t)
            (rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t dst,
             uint32_t tag, rte_group_t group);

typedef int (*rte_be_send_nbcb_fn_t)
            (rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t dst,
             uint32_t tag, int flags, rte_group_t group,
             rte_request_cb_t cb_fn, void * cb_data);

typedef int (*rte_be_barrier_fn_t)(rte_group_t group);

typedef int (*rte_be_srs_session_create_fn_t)
            (rte_group_t group, int tag, rte_srs_session_t *session);

typedef int (*rte_be_srs_session_destroy_fn_t)(rte_srs_session_t session);

typedef int (*rte_be_srs_get_data_fn_t)
            (rte_srs_session_t session, rte_ec_handle_t peer,
             rte_srs_key_t key, void **value, int *size);

typedef int (*rte_be_srs_set_data_fn_t)
            (rte_srs_session_t session, rte_srs_key_t key,
             rte_iovec_t *value, int elems);

typedef int (*rte_be_srs_exchange_data_fn_t)(rte_srs_session_t session);

typedef void (*rte_be_progress_fn_t)(void);

/* This is the lookup table into the back end implementation */
struct rte_back_end_fn_table_s
{
    rte_be_init_fn_t                rte_be_init;
    rte_be_finalize_fn_t            rte_be_finalize;
    rte_be_abort_fn_t               rte_be_abort;
    rte_be_get_my_ec_fn_t           rte_be_get_my_ec;
    rte_be_group_index_to_ec_fn_t   rte_be_group_index_to_ec;
    rte_be_group_size_fn_t          rte_be_group_size;
    rte_be_group_rank_fn_t          rte_be_group_rank;
    rte_be_get_ec_locality_fn_t     rte_be_get_ec_locality;
    rte_be_cmp_ec_fn_t              rte_be_cmp_ec;
    rte_be_get_ec_index_fn_t        rte_be_get_ec_index;
    rte_be_get_ec_node_name_fn_t    rte_be_get_ec_node_name;
    rte_be_get_ec_hostname_fn_t     rte_be_get_ec_hostname;
    rte_be_get_session_dir_fn_t     rte_be_get_session_dir;
    rte_be_cancel_fn_t              rte_be_cancel;
    rte_be_unpack_fn_t              rte_be_unpack;
    rte_be_recv_fn_t                rte_be_recv;
    rte_be_recv_nbcb_fn_t           rte_be_recv_nbcb;
    rte_be_send_fn_t                rte_be_send;
    rte_be_send_nbcb_fn_t           rte_be_send_nbcb;
    rte_be_barrier_fn_t             rte_be_barrier;
    rte_be_srs_session_create_fn_t  rte_be_srs_session_create;
    rte_be_srs_session_destroy_fn_t rte_be_srs_session_destroy;
    rte_be_srs_get_data_fn_t        rte_be_srs_get_data;
    rte_be_srs_set_data_fn_t        rte_be_srs_set_data;
    rte_be_srs_exchange_data_fn_t   rte_be_srs_exchange_data;
    rte_be_progress_fn_t            rte_be_progress;
};

struct rte_back_end_fn_table_s rte_back_end_fn_table;

int rte_init(int *argc, char ***argv, rte_group_t *out_group)
{
    /* we need to figure out which back end started us and initialize the
       table accordingly */

    /* for now we hard code orte */
    rte_back_end_fn_table.rte_be_init                = rte_orte_init;
    rte_back_end_fn_table.rte_be_finalize            = rte_orte_finalize;
    rte_back_end_fn_table.rte_be_abort               = rte_orte_abort;
    rte_back_end_fn_table.rte_be_get_my_ec           = rte_orte_get_my_ec;
    rte_back_end_fn_table.rte_be_group_index_to_ec   = rte_orte_group_index_to_ec;
    rte_back_end_fn_table.rte_be_group_size          = rte_orte_group_size;
    rte_back_end_fn_table.rte_be_group_rank          = rte_orte_group_rank;
    rte_back_end_fn_table.rte_be_get_ec_locality     = rte_orte_get_ec_locality;
    rte_back_end_fn_table.rte_be_cmp_ec              = rte_orte_cmp_ec;
    rte_back_end_fn_table.rte_be_get_ec_index        = rte_orte_get_ec_index;
    rte_back_end_fn_table.rte_be_get_ec_node_name    = rte_orte_get_ec_node_name;
    rte_back_end_fn_table.rte_be_get_ec_hostname     = rte_orte_get_ec_hostname;
    rte_back_end_fn_table.rte_be_get_session_dir     = rte_orte_get_session_dir;
    rte_back_end_fn_table.rte_be_cancel              = rte_orte_cancel;
    rte_back_end_fn_table.rte_be_unpack              = rte_orte_unpack;
    rte_back_end_fn_table.rte_be_recv                = rte_orte_recv;
    rte_back_end_fn_table.rte_be_recv_nbcb           = rte_orte_recv_nbcb;
    rte_back_end_fn_table.rte_be_send                = rte_orte_send;
    rte_back_end_fn_table.rte_be_send_nbcb           = rte_orte_send_nbcb;
    rte_back_end_fn_table.rte_be_barrier             = rte_orte_barrier;
    rte_back_end_fn_table.rte_be_srs_session_create  = rte_orte_srs_session_create;
    rte_back_end_fn_table.rte_be_srs_session_destroy = rte_orte_srs_session_destroy;
    rte_back_end_fn_table.rte_be_srs_get_data        = rte_orte_srs_get_data;
    rte_back_end_fn_table.rte_be_srs_set_data        = rte_orte_srs_set_data;
    rte_back_end_fn_table.rte_be_srs_exchange_data   = rte_orte_srs_exchange_data;
    rte_back_end_fn_table.rte_be_progress            = rte_orte_progress;

    return rte_back_end_fn_table.rte_be_init(argc, argv, out_group);
}

int rte_finalize(void)
{
    return rte_back_end_fn_table.rte_be_finalize();
}

void rte_abort(int error_code, char *exit_description, ...)
{
    va_list arglist;
    va_start(arglist, exit_description);

    return rte_back_end_fn_table.rte_be_abort(error_code,
                                              exit_description, arglist);
}

rte_ec_handle_t rte_get_my_ec(void)
{
    return rte_back_end_fn_table.rte_be_get_my_ec();
}

rte_ec_handle_t rte_group_index_to_ec(rte_group_t group,
                                      rte_node_index_t index)
{
    return rte_back_end_fn_table.rte_be_group_index_to_ec(group, index);
}

int rte_group_size(rte_group_t group)
{
    return rte_back_end_fn_table.rte_be_group_size(group);
}

int rte_group_rank(rte_group_t group)
{
    return rte_back_end_fn_table.rte_be_group_rank(group);
}

int rte_get_ec_locality(rte_ec_handle_t ec_handle)
{
    return rte_back_end_fn_table.rte_be_get_ec_locality(ec_handle);

}

int rte_cmp_ec(rte_ec_handle_t ec_handle_one,
               rte_ec_handle_t ec_handle_two)
{
    return rte_back_end_fn_table.rte_be_cmp_ec(ec_handle_one, ec_handle_two);

}

rte_node_index_t rte_get_ec_index(rte_group_t group,
                                  rte_ec_handle_t ec_handle)
{
    return rte_back_end_fn_table.rte_be_get_ec_index(group, ec_handle);

}

char * rte_get_ec_node_name(rte_ec_handle_t ec_handle)
{
    return rte_back_end_fn_table.rte_be_get_ec_node_name(ec_handle);

}

char * rte_get_ec_hostname(rte_ec_handle_t ec_handle)
{
    return rte_back_end_fn_table.rte_be_get_ec_hostname(ec_handle);

}

char * rte_get_session_dir(rte_ec_handle_t ec_handle)
{
    return rte_back_end_fn_table.rte_be_get_session_dir(ec_handle);

}

int rte_cancel(rte_ec_handle_t  peer, int tag)
{
    return rte_back_end_fn_table.rte_be_cancel(peer, tag);

}

int rte_unpack(rte_iovec_t *data, void *src, uint32_t *offset)
{
    return rte_back_end_fn_table.rte_be_unpack(data, src, offset);

}

int rte_recv(rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t src,
             uint32_t tag, rte_group_t group)
{
    return rte_back_end_fn_table.rte_be_recv(iov, cnt, src, tag, group);

}

int rte_recv_nbcb(rte_ec_handle_t src, uint32_t tag, int flags,
                  rte_group_t group, rte_request_cb_t cb_fn, void * cb_data)
{
    return rte_back_end_fn_table.rte_be_recv_nbcb (src, tag, flags, group,
                                                   cb_fn, cb_data);

}

int rte_send(rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t dst,
             uint32_t tag, rte_group_t group)
{
    return rte_back_end_fn_table.rte_be_send(iov, cnt, dst, tag, group);

}

int rte_send_nbcb(rte_iovec_t *iov, uint32_t cnt, rte_ec_handle_t dst,
                  uint32_t tag, int flags, rte_group_t group,
                  rte_request_cb_t cb_fn, void * cb_data)
{
    return rte_back_end_fn_table.rte_be_send_nbcb(iov, cnt, dst, tag, flags,          
                                                  group, cb_fn, cb_data);

}

int rte_barrier(rte_group_t group)
{
    return rte_back_end_fn_table.rte_be_barrier(group);

}

int rte_srs_session_create(rte_group_t group, int tag,
                           rte_srs_session_t *session)
{
    return rte_back_end_fn_table.rte_be_srs_session_create(group, tag, session);

}

int rte_srs_session_destroy(rte_srs_session_t session)
{
    return rte_back_end_fn_table.rte_be_srs_session_destroy(session);

}

int rte_srs_get_data(rte_srs_session_t session, rte_ec_handle_t peer,
                     rte_srs_key_t key, void **value, int *size)
{
    return rte_back_end_fn_table.rte_be_srs_get_data(session, peer, key,
                                                     value, size);

}

int rte_srs_set_data(rte_srs_session_t session, rte_srs_key_t key,
                     rte_iovec_t *value, int elems)
{
    return rte_back_end_fn_table.rte_be_srs_set_data(session, key, value, elems);

}

int rte_srs_exchange_data(rte_srs_session_t session)
{
    return rte_back_end_fn_table.rte_be_srs_exchange_data(session);
}

void rte_progress(void)
{
    return rte_back_end_fn_table.rte_be_progress();
}
