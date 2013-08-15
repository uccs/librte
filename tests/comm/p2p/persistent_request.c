/*
 * Copyright (c)           2012 UT-Battelle, LLC. All rights reserved.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

/*
 * Unit test for the "persistent request" interface
 * NOTE: the name persistent request is temporary 
 * USAGE: call the unit test with {rte,stci}run -np X persistent_request_test
 *        where X >= 2
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "rte.h"

rte_group_t group;

int done = 0, group_size;

char *aname_str_self, *aname_str_source, *aname_str_dest;

int called_back = 0;

int *agents_called_back = NULL;

void request_complete_fn (int status, rte_ec_handle_t peer, void *data_buffer, int tag, void *context);

void request_complete_fn (int status, rte_ec_handle_t peer, void *data_buffer, int tag, void *context) 
{
    char recvbuf[13];
    int rc;
    uint32_t offset = 0;
    struct rte_iovec_t my_tlv;
    int peer_index;
    
    my_tlv.iov_base = recvbuf;
    my_tlv.type     = &rte_int1;
    my_tlv.count    = 13;
    
    printf ("request_complete_fn");
    
    rc = rte_unpack (&my_tlv, data_buffer, &offset);
    if (RTE_SUCCESS != rc) {
        printf("We have a problem\n"); fflush(stdout);
    }
    
    aname_str_source = rte_get_ec_node_name (peer);
   
    fprintf (stderr, 
             "%s callback handler (%d). I received \"%s\" from %s\n", 
             aname_str_self, ++called_back, recvbuf, aname_str_source);
    fflush(stdout);
    
    my_tlv.iov_base = "hello peer";
    my_tlv.count = 11;
    
    peer_index = rte_get_ec_index (group, peer);
    
    if (!agents_called_back[peer_index]) {
        rte_send (&my_tlv, 1, peer, 1, group);
        agents_called_back[peer_index] = 1;
    }
    
    done++;
    if (done == group_size) {
        printf ("cancel request");
        rte_cancel (RTE_ANYSOURCE, 1);
    }
    
    free (aname_str_source);
    
}

int main (int argc, char **argv) {
    int rc = 0;
    int my_rank, i;
    struct rte_iovec_t my_tlv;
    char *sendstr = "Hello World!";
    rte_request_handle_t request;
    
    rte_init (&argc, &argv, &group);
    
    {
        char* value;
        value = getenv("RTE_DEBUG");
        
        if (NULL != value && 0 == strcmp(value, "1")) {
            char hostname[256];
            int value=1;
            gethostname (hostname, sizeof(hostname));
            printf ("Please attach debugger on hostname:%s to PID:%d, argc:%d",
                    hostname, getpid(), argc);
            while (value++) {
                sleep (1);
            }
        }
    }
    
    /* get the group size */
    group_size = rte_group_size (group);
    
    /* get my rank */
    my_rank    = rte_group_rank (group);
    
    /* get printeble string for my processes name */
    aname_str_self = rte_get_ec_node_name(rte_get_my_ec());
    
    /* initialize the called back list, to track the agents that called back */
    agents_called_back = calloc ( group_size, sizeof (int) );
    
    my_tlv.type   = &rte_int1;
    my_tlv.count  = 13;
    
    fprintf (stderr, "%s I am beginning!\n", 
             rte_get_ec_node_name(rte_get_my_ec()));
    
    /* create a persistent request object */
    rc = rte_recv_nbcb (RTE_ANYSOURCE, 1, RTE_RECV_REQUEST_PERSISTENT, group,
                        &request_complete_fn, NULL);
    
    if (RTE_SUCCESS != rc) {
        fprintf (stderr, "Error: failed to allocate request\n");
        exit (-1);
    }

    my_tlv.iov_base = sendstr;
    
    /* send a message to all peers */
    for (i = 0; i < group_size; i++) {
        /* dont send to myself */
        if (my_rank == i)
            continue;
        
        aname_str_dest = rte_get_ec_node_name(rte_group_index_to_ec(group,i));
        if (NULL == aname_str_dest) {
            fprintf (stderr, "Error: failed to get EC string\n");
            exit (-1);
        }

        fprintf (stderr, "%s sending to %s...\n", 
                 aname_str_self, aname_str_dest);

        rte_send (&my_tlv, 1, rte_group_index_to_ec(group, i), 1, group);
        if (NULL != aname_str_dest) {
            free (aname_str_dest);
            aname_str_dest = NULL;
        }
    }
    
    while (done < group_size) {
        rte_progress();
    }

    fprintf (stderr, "%s I am done!\n", 
             rte_get_ec_node_name(rte_get_my_ec()));
    
    rte_finalize ();
   
    if (NULL != aname_str_self)
        free (aname_str_self);
    
    return rc;
}

