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

#ifndef RTE_H
#define RTE_H

#include <stdint.h>
#include <stdbool.h>


/*****************/
/*               */
/* RTE interface */
/* V 0.2         */
/*               */
/*****************/

/**
 * @file
 *
 * @author Pavel Shamis
 * @author Swen Boehm
 * @author Geoffroy Vallee
 */

/** 
 * @addtogroup RTE
 *
 * @{
 */

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_PUBLIC
    #ifdef __GNUC__
      #define RTE_PUBLIC __attribute__ ((dllexport))
    #else
      #define RTE_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define RTE_PUBLIC __attribute__ ((dllimport))
    #else
      #define RTE_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define RTE_LOCAL
#else
  #if __GNUC__ >= 4
    #define RTE_PUBLIC __attribute__ ((visibility ("default")))
    #define RTE_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define RTE_PUBLIC
    #define RTE_LOCAL
  #endif
#endif

/****************
 * Return Codes *
 ****************/

/* Return codes */
typedef enum {
    RTE_SUCCESS = 0,
    RTE_ERROR = -1,
    RTE_ERROR_BAD_INPUT = -2,
    RTE_ERROR_OUT_OF_RESOURCE = -3,
    RTE_ERROR_NOT_IMPLEMENTED = -4,
    RTE_ERROR_NOT_FOUND = - 5,
    RTE_MAX = -255
} rte_return_code;

/*
 * Basic data types
 */

typedef struct rte_dt_t * rte_datatype_t;

/* from ompi (mpi.h) */
/*
 * When initializing global pointers to Open MPI internally-defined
 * structs, some compilers warn about type-punning to incomplete
 * types.  Therefore, when full struct definitions are unavailable
 * (when not building Open MPI), cast to an opaque (void *) pointer to
 * disable any strict-aliasing optimizations.  Don't cast to (void *)
 * when building Open MPI so that we actually get the benefit of type
 * checking (because we *do* have the full type definitions available
 * when building OMPI).
 */
RTE_PUBLIC extern const struct rte_dt_t rte_int1; /* 8 byte  */
RTE_PUBLIC extern const struct rte_dt_t rte_int2; /* 16 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_int4; /* 32 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_int8; /* 64 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_uint1;/* 8 byte  */
RTE_PUBLIC extern const struct rte_dt_t rte_uint2;/* 16 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_uint4;/* 32 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_uint8;/* 64 byte */
RTE_PUBLIC extern const struct rte_dt_t rte_float2;
RTE_PUBLIC extern const struct rte_dt_t rte_bool;

#define RTE_PREDEFINED_GLOBAL(type, global) ((type) ((void *) &(global)))

#define rte_datatype_int8_t        RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int1)
#define rte_datatype_int16_t       RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int2)
#define rte_datatype_int32_t       RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int4)
#define rte_datatype_int64_t       RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int8)
#define rte_datatype_uint8_t       RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint1)
#define rte_datatype_uint16_t      RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint2)
#define rte_datatype_uint32_t      RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint4)
#define rte_datatype_uint64_t      RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint8)
#define rte_datatype_float_t       RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_float2)
#define rte_datatype_bool_t        RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_bool)

/*****************************
 * Interface data structures *
 *****************************/

/* 
 * Run time handle - runtime agent context is associated with such a handle
 */
typedef void * rte_ec_handle_t;

/**
 * @brief Handle for a RTE group.
 */
typedef void *   rte_group_t;           /* Similar to MPI Communicator */
typedef int      rte_tag_t;
typedef uint32_t rte_node_index_t;      /* Node index */

/* RTE iovec structure.
 * In order to support heterogeneous runtime environment,
 * instead of length we have datatype (type) and number of elements of this type (count)
 *
 * the stride defines the distance between element(n)'s and element(n+1)'s start position in memory
 */
typedef struct rte_iovec_t {
    void           *iov_base;
    const struct rte_dt_t *type;
    uint32_t        count;
} rte_iovec_t;

/** 
 * @brief RTE initialization
 *
 * @details
 * The rte_init function initializes the run time environment. It shall be
 * called before calls to any other run time functions.
 *
 * @param[in]      argc       Number of arguments in argument vector
 * @param[in]      argv       The argument vector
 * @param[in/out]  out_group  Group containing all execution contexts in the 
 *                            Application
 *
 * @return rte_init returns RTE_SUCCES on success or RTE_ERROR otherwise.
 */
RTE_PUBLIC int rte_init(int *argc, char ***argv, rte_group_t *out_group);

/**
 * @brief RTE agent finalization
 *
 * @details
 * The rte_finalize function shuts down the runtime. After calling rte_fini 
 */
RTE_PUBLIC int rte_finalize(void);

/**
 * @brief RTE agent abort
 */
RTE_PUBLIC void rte_abort(int error_code, char *exit_description, ...);


/**
 * @brief Return handle to the calling processes/threads execution context
 *
 * @return rte_ec_handle_t
 */
RTE_PUBLIC rte_ec_handle_t rte_get_my_ec(void);

/**
 * @brief Group index to execution context translation
 *
 * @param[in]  group  The group to query the execution context from.
 * @param[in]  index  The index of the execution context in the group.
 *
 * @return Returns rte_ec_handle_t for the specified execution context.
 */
RTE_PUBLIC rte_ec_handle_t rte_group_index_to_ec(rte_group_t group,
                                                 rte_node_index_t index);

/* Return group size */
RTE_PUBLIC int rte_group_size(rte_group_t group);

/* returns the group rank */
RTE_PUBLIC int rte_group_rank(rte_group_t group);

/* Pasha - the codes where taken from hwloc
 * opal/mca/hwloc/hwloc.h
 */
RTE_PUBLIC extern const int RTE_PROC_LOCALITY_UNKNOWN;
RTE_PUBLIC extern const int RTE_PROC_NON_LOCAL;
RTE_PUBLIC extern const int RTE_PROC_ON_CLUSTER;
RTE_PUBLIC extern const int RTE_PROC_ON_CU;
RTE_PUBLIC extern const int RTE_PROC_ON_NODE;
RTE_PUBLIC extern const int RTE_PROC_ON_BOARD;
RTE_PUBLIC extern const int RTE_PROC_ON_NUMA;
RTE_PUBLIC extern const int RTE_PROC_ON_SOCKET;
RTE_PUBLIC extern const int RTE_PROC_ON_L3CACHE;
RTE_PUBLIC extern const int RTE_PROC_ON_L2CACHE;
RTE_PUBLIC extern const int RTE_PROC_ON_L1CACHE;
RTE_PUBLIC extern const int RTE_PROC_ON_CORE;
RTE_PUBLIC extern const int RTE_PROC_ON_HWTHREAD;
RTE_PUBLIC extern const int RTE_PROC_ALL_LOCAL;

RTE_PUBLIC int rte_get_ec_locality(rte_ec_handle_t ec_handle);

/*
 * The function checks if the 2 ECs are identical. 
 * 0 - identical , 1  index of one is larger then index of two,
 * -1 - index of one is less then two
 */
RTE_PUBLIC int rte_cmp_ec(rte_ec_handle_t ec_handle_one, 
                          rte_ec_handle_t ec_handle_two);

/**
 * return a pointer to a string holding the RTE's index for the
 *  end-point corresponding to the execution-context's handle.
 *  A NULL return handle indicates an error.
 *  The assumption is that the pointer is managed by the RT, and
 *  is valid for the lifetime of the specified execution-context.
 */
RTE_PUBLIC rte_node_index_t rte_get_ec_index(rte_group_t group,
                                             rte_ec_handle_t ec_handle);

/*
 * return a pointer to a string holding the RTE's "node" name.
 *  The assumption is that the pointer is managed by the RT, and
 *  is valid for the lifetime of the specified execution-context.
 */
RTE_PUBLIC char * rte_get_ec_node_name(rte_ec_handle_t ec_handle);

/*
 * return a pointer to a string holding the RTE's HOSTNAME 
 * (not necessarily identical to "node" name)
 */
RTE_PUBLIC char * rte_get_ec_hostname(rte_ec_handle_t ec_handle);

/*
 * return a pointer to a string holding the full path to RTE's session directory 
 */
RTE_PUBLIC char * rte_get_session_dir(rte_ec_handle_t ec_handle);

/**
 * @addtogroup rte_request Request functions
 *
 * @{
 */

/**
 * @brief Handle to a request object.
 */
typedef void * rte_request_handle_t;  /* Publish / Subscribe request handle */


/* SB: I am not sure if cbdata should be a void pointer or rather a datatype 
 *     giving detailed information about the context that triggert the callback 
 *     (source, tag, etc.)
 */

typedef void (*rte_request_cb_t) (int status, rte_ec_handle_t peer,
                                  void * data_buffer, int tag, void *cbdata);

RTE_PUBLIC extern const int RTE_RECV_REQUEST_PERSISTENT;

/**
 * @brief cancel a request
 */
RTE_PUBLIC int rte_cancel(rte_ec_handle_t  peer, int tag);

/**
 * @brief unpack data into a user defined buffer
 */
RTE_PUBLIC int rte_unpack(rte_iovec_t *data,
                          void *src,
                          uint32_t *offset);

/**
 * @addtogroup rte_p2p Point to Point Messaging 
 * @{
 *
 * @details
 * The point to point messaging layer enables the user to send data to another
 * peer. The interface provides blocking and non blocking semantics. The
 * progress of non blocking calls can be tracked with a request object.
 */

RTE_PUBLIC extern rte_ec_handle_t RTE_ANYSOURCE;


/**
 * @brief Blocking receive
 *
 * @param[in] rte_iovec_t     iovec array of data elements
 * @param[in] count           number of elements in iovec array
 * @param[in] rte_ec_handle_t source
 * @param[in] uint32_t        tag
 * @param[in] rte_group_t     group
 */
RTE_PUBLIC int rte_recv(rte_iovec_t *iov,
                        uint32_t cnt,
                        rte_ec_handle_t src,
                        uint32_t tag,
                        rte_group_t group);
/**
 * @brief Start a non blocking receive with callback (no request)
 *
 * @param[in] rte_ec_handle_t    source
 * @param[in] uint32_t           tag
 * @param[in] rte_request_type_t flags
 * @param[in] rte_group_t        group
 * @param[in] rte_request_cb_t   callback function
 * @param[in] void *             callback data
 */
RTE_PUBLIC int rte_recv_nbcb(rte_ec_handle_t src,
                             uint32_t tag,
                             int flags,
                             rte_group_t group,
                             rte_request_cb_t cb_fn,
                             void * cb_data);
/**
 * @brief Blocking send
 *
 * @param[in] rte_iovec_t     iovec array of data elements
 * @param[in] count           number of elements in iovec array
 * @param[in] rte_ec_handle_t dest
 * @param[in] uint32_t        tag
 * @param[in] rte_group_t     group
 */
RTE_PUBLIC int rte_send(rte_iovec_t *iov,
             uint32_t cnt,
             rte_ec_handle_t dst,
             uint32_t tag,
             rte_group_t group);

/**
 * @brief Blocking send with function callback
 *
 */
RTE_PUBLIC int rte_send_nbcb(rte_iovec_t *iov,
        uint32_t cnt,
        rte_ec_handle_t dst,
        uint32_t tag,
        int flags,
        rte_group_t group,
        rte_request_cb_t cb_fn,
        void * cb_data);

/** @} */ /* end rte_p2p */
/**
 * @addtogroup ret_coll Collective Communication
 * @{
 */

/**
 * @brief Barrier function
 *
 * @param[in] group                the group to syncronize
 *
 * @return RTE_SUCCESS on success, error code otherwise
 */
RTE_PUBLIC int rte_barrier(rte_group_t group);
/** @} */ /* end group rte_coll */

/*--                           SRS Interface                                --*/

/**
 * @addtogroup SRS Storage Rertieval System
 * @{
 *
 * @details
 * The Storage Rertieval System (SRS) is intended to provide the means
 * to communicate key - value pairs. The user can publish and/or subscribe
 * arbitary values identified by a key.
 *
 * The data shall be stored in a global registry unless the user limits the 
 * participants to a specific set of peers. To limit the peers participating in 
 * a srs exchange the user can create a session.
 *
 * example publish code
 *
 * @code
 * module1_init (...)
 * {
 *
 *   rte_srs_session_t session1
 *   rte_srs_session_create (..., &session1);
 *
 *   ...
 *
 *   // here I publish data for the specified session and key
 *   rte_srs_set_data (session1, "key1.1", valueptr, elems);
 * }
 *
 * module2_init (...)
 * {
 *   rte_srs_session_t session2;
 *   rte_srs_session_create (..., &session2);
 *
 *   ...
 *
 *   // here I publish data for the specified session and key
 *   rte_srs_set_data (session2, "key2.1", valueptr, elems);
 * }
 *
 * module3_init (...)
 * {
 *   rte_srs_session_t session3;
 *   rte_srs_session_create (..., &session3);
 *
 *   ...
 *
 *   // here I publish data for the specified session and key
 *   rte_srs_set_data (session3, "key3.1", valueptr, elems);
 * }
 *
 * module1_do_something (...)
 * {
 *   rte_srs_get_data (session1, peer, "key1.1", &buffer);
 * }
 *
 * module2_do_something (...)
 * {
 *   rte_srs_get_data (session2, peer, "key2.1", &buffer);
 * }
 *
 * module3_do_something (...)
 * {
 *   rte_srs_get_data (session3, peer, "key3.1", &buffer);
 * }
 *
 * rte_init (...)
 * {
 *   module1_init (...);
 *   module2_init (...);
 *   module3_init (...);
 *
 *   rte_srs_exchange_data_nb (session1, &request1);
 *   rte_srs_exchange_data_nb (session2, &request2);
 *   rte_srs_exchange_data_nb (session3, &request3);
 *   ...
 *
 *   rte_wait (request1);
 *   rte_wait (request2);
 *   rte_wait (request3);
 *
 *   module1_do_something (...);
 *   module2_do_something (...);
 *   module3_do_something (...);
 * }
 *
 * @endcode
 */

/**
 * @brief An opaque handle to identify the session for the exchange
 */
typedef void * rte_srs_session_t;

/**
 * @brief The key for the key-value pair
 */
typedef char * rte_srs_key_t;

/**
 * @brief Create a srs session
 *
 * @param[in]     group    the group this session is limited to
 * @param[in]     tag      an arbitary tag
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_srs_session_create(rte_group_t group,
                                      int tag,
                                      rte_srs_session_t *session);

/**
 * @brief destroy a previously created srs session
 *
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS on success, RTE_ERROR otherwise
 */
RTE_PUBLIC int rte_srs_session_destroy(rte_srs_session_t session);

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
RTE_PUBLIC int rte_srs_get_data(rte_srs_session_t session,
                                rte_ec_handle_t peer,
                                rte_srs_key_t key,
                                void **value,
                                int *size);

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
RTE_PUBLIC int rte_srs_set_data(rte_srs_session_t session,
                                rte_srs_key_t key,
                                rte_iovec_t *value,
                                int elems);

/**
 * @brief Exchange the data in the SRS cache.
 *
 * @param[in]  session  NULL for global exchange, rte_srs_session_t for
 *                      scoped exchange
 * @param[out] request the request object
 * @return returns RTE_SUCCESS on success or an error code in case of error
 * Pasha: depricated
 */
RTE_PUBLIC int rte_srs_exchange_data(rte_srs_session_t session);

RTE_PUBLIC void rte_progress(void);

/** @} */ /* end group srs */
/** @} */ /* end group RTE */

#endif /* RTE_H */
