/*
 * Copyright (c) 2012       Oak Ridge National Laboratory.
 *                          All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "rte/include/rte.h"
#include "stci/event/stci_event_api.h"

/* FIXME: we need this for the anysource definition. as soon as anysource
 * is defined, we can remove this include */
#include "stci/runtime/stci_runtime.h"
#include "stci/runtime/stci_progress.h"

/* note, this is a hack and needs to be fixed when there is a stci level implementation */
STCI_Process_name_t STCI_ANYSOURCE = {(STCI_JOBTYPE_TA << STCI_JOBID_BITS) | STCI_JOBID_WILDCARD,
    (STCI_VPIDEPOCH_WILDCARD << STCI_VPID_BITS) | STCI_VPID_WILDCARD};

rte_group_t group_world;

int done = 0;
int called_back = 0;
char *aname_str_self, *aname_str_source, *aname_str_dest;
int *agents_called_back = NULL;

/* forward declaration for callback function */
void request_complete_fn (rte_request_handle_t      req,
                          int                       status,
                          int                       tag,
                          rte_ec_handle_t           peer,
                          void                     *data_buffer,
                          void                     *context);

int main (int argc, char **argv)
{
    int                     rc;
    rte_srs_session_t       session1  = NULL;
    rte_request_handle_t    pub_hdl1  = NULL;
    rte_request_handle_t    request   = NULL;
    rte_ec_handle_t         target_ec;
    
    rte_iovec_t             iov[1];
    rte_iovec_t             my_iov;
    
    int group_size, my_rank, i, buff_size;
    uint32_t offset;
    
    char buf1[] = "Hello World!";
    int retval = EXIT_SUCCESS;
    void *databuff = NULL;
    
    /* initialize the runtime environment */
    rc = rte_init (&argc, &argv, &group_world);
    if (RTE_SUCCESS != rc) {
        exit (-1);
    }
    
    /* get the group size */
    group_size = rte_group_size (group_world);
    
    /* get my rank */
    my_rank    = rte_group_rank (group_world);
    
    /* get printeble string for my processes name */
    aname_str_self = rte_ec_to_str(rte_get_my_ec());

    fprintf (stdout, "Setting up the modex...\n");
    
    /* create a modex session object */
    rc = rte_srs_session_create (group_world, 0, &session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to allocate publish request\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }
    
    /* initialize the iovec to send data trough the modex */
    iov[0].iov_base = &buf1;
    iov[0].type     = rte_datatype_int1;
    iov[0].stride   = 1;
    iov[0].count    = strlen (buf1) +1;
    
    /* associate the data in the iov to a session key */
    rc = rte_srs_set_data (session1, "Key1", iov, 1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to set data on session1\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }
    
    rc = rte_srs_exchange_data_nb (session1, &pub_hdl1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to prepare data publishing\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }
    
    /* Just to make sure the usage of rte_progress() does not create problems,
       we call it explicitely a few times. */
    for (i = 0; i < rte_group_size(group_world); i++) {
        rte_progress ();
    }

    fprintf (stdout, "Waiting for the data to arrive...\n");
    
    rc = rte_wait (pub_hdl1, NULL);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Modex wait failed\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }
    
    for (i = 0; i < rte_group_size(group_world); i++) {
        
        target_ec = rte_group_index_to_ec (group_world, i);
        rc = rte_srs_get_data (session1, target_ec, "Key1",
                               &databuff, &buff_size);
        
        printf ("(key1) Buffer size = %d\n", buff_size);
        
        my_iov.iov_base  = NULL;
        my_iov.count     = 0;
        my_iov.stride    = 1;
        my_iov.type      = rte_datatype_int1;
        
        offset = 0;
        
        if (0 < buff_size) {
            rte_unpack (&my_iov, databuff, &offset);
            
            if (NULL != my_iov.iov_base)
                printf ("%s\n", (char*)my_iov.iov_base);
            else
                printf ("Warning: my_iov.iov_base is NULL!\n");
        }
        
        if (rc != RTE_SUCCESS) {
            fprintf (stderr, "Error: Impossible to subscribe for data\n");
            retval = EXIT_FAILURE;
            goto cleanup;
        }
    }
    
    /* clean up the modex session */
    rc = rte_srs_session_destroy (session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to destroy publish request\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }
    
    fprintf (stdout, "Data received, starting persistent request test!\n");
    
    /* initialize the called back list, to track the agents that called back */
    agents_called_back = calloc ( group_size, sizeof (int) );
    
    /* create a persistent request object */
    rc = rte_allocate_request (1,
                               RTE_RECV_REQUEST_PERSISTENT,
                               &request_complete_fn,
                               NULL,
                               group_world,
                               &request);
    if (RTE_SUCCESS != rc) {
        fprintf (stderr, "Error: failed to allocate request\n");
        exit (-1);
    }
    
    /* post the persistent request */
    rc = rte_post_recv_request (request, &STCI_ANYSOURCE);
    if (RTE_SUCCESS != rc) {
        fprintf (stderr, "Error: failed to post request\n");
        exit (-1);
    }
    
    my_iov.type   = &rte_int1;
    my_iov.count  = strlen (buf1) + 1;
    my_iov.stride = 1; /*rte_get_datatype_size (&rte_int1); */
    my_iov.iov_base = buf1;
    
    /* send a message to all peers */
    for (i = 0; i < group_size; i++) {
        /* dont send to myself */
        if (my_rank == i)
            continue;
        
        aname_str_dest = rte_ec_to_str(rte_group_index_to_ec(group_world,i));
        if (NULL == aname_str_dest) {
            fprintf (stderr, "Error: failed to get EC string\n");
            exit (-1);
        }
        
        fprintf (stderr, "%s sending to %s...\n",
                 aname_str_self, aname_str_dest);
        
        rte_send (&my_iov, 1, rte_group_index_to_ec(group_world, i), 1, group_world);
        if (NULL != aname_str_dest) {
            free (aname_str_dest);
            aname_str_dest = NULL;
        }
    }
    
    while (done < group_size) {
        stci_progress();
    }
    
    fprintf (stderr, "%s I am done!\n",
             rte_ec_to_str(rte_get_my_ec()));
    
cleanup:
    rte_finalize ();
    
    if (NULL != aname_str_self)
        free (aname_str_self);
    
    return retval;
    
}

void request_complete_fn (rte_request_handle_t  req, int status, int tag, rte_ec_handle_t peer, void *data_buffer, void *context)
{
    char recvbuf[13];
    int rc;
    uint32_t offset = 0;
    struct rte_iovec_t my_tlv;
    int peer_index;
    
    my_tlv.iov_base = recvbuf;
    my_tlv.type     = &rte_int1;
    my_tlv.count    = 13;
    my_tlv.stride   = 1; /*rte_get_datatype_size (&rte_int1); */
    
    rc = rte_unpack (&my_tlv, data_buffer, &offset);
    if (RTE_SUCCESS != rc) {
        printf("Houston, we have a problem\n"); fflush(stdout);
    }
    
    aname_str_source = rte_ec_to_str (peer);
    
    fprintf (stderr,
             "%s callback handler (%d). I received \"%s\" from %s\n",
             aname_str_self, ++called_back, recvbuf, aname_str_source);
    fflush(stdout);
    
    my_tlv.iov_base = "hello peer";
    my_tlv.count = 11;
    
    peer_index = rte_get_ec_index (group_world, peer);
    
    if (!agents_called_back[peer_index]) {
        rte_send (&my_tlv, 1, peer, 1, group_world);
        agents_called_back[peer_index] = 1;
    }
    
    done++;
    
    free (aname_str_source);
    
}
