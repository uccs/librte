/*
 * Copyright (c) 2011-2013  Oak Rigde National Laboratory
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
 * @addtogroup rte RTE
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

/**
 * @brief  RTE error codes 
 *
 * @details Most RTE functions will return one of these codes to
 *          indicate successful completion or if an error occurred.
 */
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

/**
 * @brief Data type description.
 *
 * @details The rte_datatype_t type is used to describe the data stored
 *          in a buffer.
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

RTE_PUBLIC extern const struct rte_dt_t rte_int1; /*< 8 bit  */
RTE_PUBLIC extern const struct rte_dt_t rte_int2; /* 16 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_int4; /* 32 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_int8; /* 64 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_uint1;/* 8 bit  */
RTE_PUBLIC extern const struct rte_dt_t rte_uint2;/* 16 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_uint4;/* 32 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_uint8;/* 64 bit */
RTE_PUBLIC extern const struct rte_dt_t rte_float2;
RTE_PUBLIC extern const struct rte_dt_t rte_bool;

#define RTE_PREDEFINED_GLOBAL(type, global) ((type) ((void *) &(global)))

#define rte_datatype_int8_t   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int1)
#define rte_datatype_int16_t  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int2)
#define rte_datatype_int32_t  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int4)
#define rte_datatype_int64_t  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int8)
#define rte_datatype_uint8_t  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint1)
#define rte_datatype_uint16_t RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint2)
#define rte_datatype_uint32_t RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint4)
#define rte_datatype_uint64_t RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint8)
#define rte_datatype_float_t  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_float2)
#define rte_datatype_bool_t   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_bool)


/* This defines remain for compatibility reasons. They may be removed at any time
 * Please use the bit sized defines in new code */
#define rte_datatype_int1   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int1)
#define rte_datatype_int2   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int2)
#define rte_datatype_int4   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int4)
#define rte_datatype_int8   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_int8)
#define rte_datatype_uint1  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint1)
#define rte_datatype_uint2  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint2)
#define rte_datatype_uint4  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint4)
#define rte_datatype_uint8  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_uint8)
#define rte_datatype_float  RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_float2)
#define rte_datatype_bool   RTE_PREDEFINED_GLOBAL (rte_datatype_t, rte_bool)

/*****************************
 * Interface data structures *
 *****************************/

/**
 * @brief Execution Context (EC) type. 
 * @details A runtime execution context handle is associated with every
 *          process in the application.
 */
typedef void * rte_ec_handle_t;

/**
 * @brief Handle for a RTE group.
 */
typedef void *   rte_group_t;           /* Similar to MPI Communicator */
typedef int      rte_tag_t;

/**
 * @brief   The node index.
 * @details This type is used for the index in of an EC in the
 *          application or group.
 */
typedef uint32_t rte_node_index_t;      /* Node index */

/**
 * @brief RTE iovec structure.
 * @details In order to support heterogeneous runtime environment, the
 *          rte_iovec_t contains the type of the data and the number of
 *          elements of that type.
 */
typedef struct rte_iovec_t {
    void           *iov_base;    /**< pointer to the data */
    const struct rte_dt_t *type; /**< The data type of the elements in
                                      the iovec */
    uint32_t        count;       /**< Number of elements in the vector */
} rte_iovec_t;

/**
 * @brief RTE initialization
 *
 * @details The rte_init function initializes the run time environment.
 *          It shall be called before calls to any other run time functions.
 *
 * @param[in]     argc       Number of arguments in argument vector
 * @param[in]     argv       The argument vector
 * @param[in,out] out_group  Group containing all execution contexts in the
 *                            Application
 * @note argc may be 0 and argv may be NULL
 *
 * @return RTE_SUCCESS On successful initialization.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_init(int *argc, char ***argv, rte_group_t *out_group);
/* proposed change:
   RTE_PUBLIC int rte_init (void);
 */


/**
 * @brief RTE agent finalization
 *
 * @details
 * The rte_finalize function shuts down the runtime. After calling
 * rte_finalize no more calls to rte should be made.
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_finalize(void);

/**
 * @brief RTE agent abort
 * @details This function aborts the current execution context and
 *          possibly the application.
 *          Note: Aborting the application is not guaranteed. This
 *          depends on the back end. Instead of aborting the application
 *          a back end implementation might notify the other ECs.
 *
 * @return RTE_SUCCESS On successful initialization.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC void rte_abort(int error_code, char *exit_description, ...);

RTE_PUBLIC uint32_t rte_get_job_id (void);

/**
 * @brief Return handle to the calling processes/threads execution context
 *
 * @return The rte_ec_handle_t of the calling process.
 */
RTE_PUBLIC rte_ec_handle_t rte_get_my_ec(void);

/**
 * @brief Group index to execution context translation
 *
 * @param[in]  group  The group to query the execution context from.
 * @param[in]  index  The index of the execution context in the group.
 *
 * @return A rte_ec_handle_t for the ec at the given rank in the group.
 */
RTE_PUBLIC rte_ec_handle_t rte_group_index_to_ec(rte_group_t group,
                                                 rte_node_index_t index);

/**
 * @brief Return the size of the given group
 *
 * @param[in]  group  The group to query for its size.
 *
 * @return The size of the given group
 */
RTE_PUBLIC int rte_group_size(rte_group_t group);

/**
 * @brief Get the rank in a given group.
 *
 * @param[in]  group  The group to query for its size.
 *
 * @return The rank of the calling process in the given group
 */
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

/**
 * @brief Return the locality of a given execution context.
 */
RTE_PUBLIC int rte_get_ec_locality(rte_ec_handle_t ec_handle);

/**
 * @brief Compare two execution contexts.
 *
 * @param[in] ec_handle_one The first handle to an execution context.
 * @param[in] ec_handle_two The second handle to an execution context.
 *
 * The function checks if the two ECs are identical.
 * @return  0 if identical
 * @return  1 if the index of ec_handle_one is larger then index of
 *          ec_handle_two,
 * @return -1 if the index of ec_handle_one is less then ec_handle_two
 */
RTE_PUBLIC int rte_cmp_ec(rte_ec_handle_t ec_handle_one,
                          rte_ec_handle_t ec_handle_two);

/**
 * @brief Get the execution contexts index in a group.
 *
 * @param[in] group      The group handle.
 * @param[in] ec_handle  The EC handle.
 *
 * @return The index of the given execution context in the given group
 */
RTE_PUBLIC rte_node_index_t rte_get_ec_index(rte_group_t group,
                                             rte_ec_handle_t ec_handle);

/**
 * @brief Get the node name for an EC.
 *
 * @param[in] ec_handle  The EC handle.
 *
 * @return A pointer to a string holding the RTE's "node" name.
 * @note The assumption is that the pointer is managed by the RT, and
 *       is valid for the lifetime of the specified execution-context.
 */
RTE_PUBLIC char * rte_get_ec_node_name(rte_ec_handle_t ec_handle);


/**
 * @brief Get the unique node identifier for an EC.
 *
 * @param[in] ec_handle  The EC handle.
 *
 * @return uint32_t the node identifier.
 */
RTE_PUBLIC uint32_t rte_get_ec_node_id(rte_ec_handle_t ec_handle);

/**
 * @brief Get the hostname for an EC.
 *
 * @param[in] ec_handle  The EC handle.
 *
 * @return pointer to a string holding the execution contexts host name
 * @note The host name is not necessarily identical to the node name
 */
RTE_PUBLIC char * rte_get_ec_hostname(rte_ec_handle_t ec_handle);

/**
 * @brief  Get path to session directory.
 *
 * @return A pointer to a string holding the full path to applications
 *         session directory
 */
 /* do we need a context here?  */
RTE_PUBLIC char * rte_get_session_dir(rte_ec_handle_t ec_handle);

typedef void (*rte_request_cb_t) (int status, rte_ec_handle_t peer,
                                  void * data_buffer, int tag, void
                                  *cbdata);

/* SB: should we rename this? There are no requests in the RTE interface
 * anymore. */
RTE_PUBLIC extern const int RTE_RECV_REQUEST_PERSISTENT;


/**
 * @brief Cancel an outstanding non blocking send/receive operation.
 *
 * @param[in] peer The peer the outstanding send/receive is for
 * @param[in] tag  The tag qualifying the send/receive operation.
 *
 * @note If there are multiple operations that match the peer, tag
 *       combination, all of them might be canceled.
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_cancel(rte_ec_handle_t  peer, int tag);

/**
 * @brief Unpack data into a user defined buffer.
 *
 * @param data
 * @param src
 * @param offset
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_unpack(rte_iovec_t *data,
                          void *src,
                          uint32_t *offset);

/*
 * @addtogroup rte_p2p Point to Point Messaging
 * @{
 *
 * @details
 * The point to point messaging layer enables the user to send data to another
 * peer. The interface provides blocking and non blocking semantics. The
 * progress of non blocking calls can be tracked with a request object.
 */

/**
 * @brief A wildcard handle that matches any source in the application
 *        context.
 */
RTE_PUBLIC extern rte_ec_handle_t RTE_ANYSOURCE;

/**
 * @brief Blocking receive
 *
 * @param[in] iov    Array of rte_iovec_t data elements.
 * @param[in] count  Number of elements in iovec array.
 * @param[in] source The source of the message.
 * @param[in] tag    The message tag.
 * @param[in] group
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_recv(rte_iovec_t *iov,
                        uint32_t count,
                        rte_ec_handle_t source,
                        uint32_t tag,
                        rte_group_t group);
/**
 * @brief Start a non blocking receive.
 *
 * @param[in] source  The source of the message.
 * @param[in] tag     The message tag.
 * @param[in] flags   The flag is used to specify whether the receive is
 *                    persistent or not. For a persistent receive set it
 *                    to RTE_RECV_REQUEST_PERSISTENT otherwise set it to 0.
 * @param[in] group
 * @param[in] cb_fn   A pointer to a function that shall be called upon
 *                    reception of the message.
 * @param[in] cb_data A pointer to a data structure qualifying the
 *                    receive operation.
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_recv_nbcb(rte_ec_handle_t source,
                             uint32_t tag,
                             int flags,
                             rte_group_t group,
                             rte_request_cb_t cb_fn,
                             void * cb_data);
/**
 * @brief Blocking send
 *
 * @param[in] iov    Array of rte_iovec_t data elements.
 * @param[in] count  Number of elements in iovec array.
 * @param[in] dest   The destination for the message.
 * @param[in] tag    The message tag.
 * @param[in] group
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_send(rte_iovec_t *iov,
             uint32_t count,
             rte_ec_handle_t dest,
             uint32_t tag,
             rte_group_t group);

/**
 * @brief Start a non-blocking send.
 *
 * @param[in] iov     Array of rte_iovec_t data elements.
 * @param[in] count   Number of elements in iovec array.
 * @param[in] dest    The source of the message.
 * @param[in] tag     The message tag.
 * @param[in] flags
 * @param[in] group
 * @param[in] cb_fn   A pointer to a function that shall be
 *                    called upon
 *                    reception of the message.
 * @param[in] cb_data A pointer to a data structure
 *                    qualifying the receive operation.
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_send_nbcb(rte_iovec_t *iov,
        uint32_t count,
        rte_ec_handle_t dest,
        uint32_t tag,
        int flags,
        rte_group_t group,
        rte_request_cb_t cb_fn,
        void * cb_data);

/**
 * @brief Barrier function
 *
 * @param[in] group                the group to synchronize
 *
 * @return RTE_SUCCESS On success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_barrier(rte_group_t group);

/*--                           SRS Interface                                --*/

/**
 * @addtogroup SRS Storage Retrieval System
 * @{
 *
 * @details
 * The Storage Retrieval System (SRS) is intended to provide the means
 * to communicate key - value pairs. The user can publish and/or subscribe
 * arbitrary values identified by a key.
 *
 * The data shall be stored in a global registry unless the user limits the
 * participants to a specific set of peers. To limit the peers participating
 * in a srs exchange the user can create a session.
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
 * @param[in]     group    The group this session is limited to.
 * @param[in]     tag      An arbitrary tag.
 * @param[in,out] session  A pointer to the session the session object.
 *
 * @return RTE_SUCCESS On Success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_srs_session_create(rte_group_t group,
                                      int tag,
                                      rte_srs_session_t *session);

/**
 * @brief destroy a previously created srs session
 *
 * @param[in,out] session  pointer to the session the session object
 *
 * @return RTE_SUCCESS On Success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_srs_session_destroy(rte_srs_session_t session);

/**
 * @brief blocking call to request for a key/value pair from a session
 *
 * @param[in]  session  NULL for global exchange, rte_srs_session_t for
 *                      scoped exchange.
 * @param[in]  peer     The peer we want the data from.
 * @param[in]  key      The key the value will be assigned to.
 * @param[out] value    A pointer to the data buffer for the given key.
 * @param[out] size     The size of the data buffer.
 *
 * @return RTE_SUCCESS On Success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_srs_get_data(rte_srs_session_t session,
                                rte_ec_handle_t peer,
                                rte_srs_key_t key,
                                void **value,
                                int *size);

/**
 * @brief blocking call to register a key/value pair with a session
 *
 * @note If the key provided already is registered with the session
 *       RTE_ERR_DUPLICATE_KEY is returned.
 *
 * @param[in]  session     NULL for global exchange, rte_srs_session_t for
 *                         scoped exchange
 * @param[in]  key         the key the value will be assigned to
 * @param[in]  value       the data stored in the srs
 * @param[in]  elems       number of elements to store in srs
 *
 * @return RTE_SUCCESS On Success.
 * @return RTE_ERROR On error.
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
 *
 * @return RTE_SUCCESS On Success.
 * @return RTE_ERROR On error.
 */
RTE_PUBLIC int rte_srs_exchange_data(rte_srs_session_t session);

RTE_PUBLIC void rte_progress(void);

/** @} */ /* end group srs */
/** @} */ /* end group RTE */

#endif /* RTE_H */
