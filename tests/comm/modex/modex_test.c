/*
 * Copyright (c) 2012-2013  Oak Ridge National Laboratory.
 *                          All rights reserved.
 *
 * Copyright (c) 2013       UT-Battelle, LLC. All rights reserved.
 *
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

#include "rte.h"

#define IOVCNT 1

int main (int argc, char **argv) {
    int rc                         = 0;
    int retval                     = EXIT_SUCCESS;
    rte_srs_session_t    session1  = NULL;
    char buf1[]                    = "PMI is";
    char buf2[10];
    char buf3[] = "not\0";
    char buf4[] = "cool";
    rte_iovec_t iov[3];
    rte_iovec_t my_iov[3];
    rte_group_t             group_world;
    rte_ec_handle_t         target_ec;
    int my_rank;
    void *databuff                  = NULL;
    int i, buff_size;
    uint32_t offset;
    char* value = NULL;

    rc = rte_init (&argc, &argv, &group_world);

    value = getenv("RTE_DEBUG");

    my_rank = rte_group_rank (group_world);

    if (NULL != value && 0 == strcmp(value, "1")) {
        char hostname[256];
        int value=1;

        gethostname (hostname, sizeof(hostname));
        fprintf (stderr,
                 "Please attach debugger on hostname:%s to PID:%d, argc:%d",
                 hostname, getpid(), argc);
        while (value++) {
            sleep (1);
        }
    }

    rc = rte_srs_session_create (group_world, 0, &session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to allocate publish request\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }

    iov[0].iov_base = &buf1;
    iov[0].type     = rte_datatype_int8_t;
    iov[0].count    = strlen (buf1) + 1;

    iov[1].iov_base = &buf3;
    iov[1].type     = rte_datatype_int8_t;
    iov[1].count    = 4;

    iov[2].iov_base = &buf4;
    iov[2].type     = rte_datatype_int8_t;
    iov[2].count    = strlen (buf4) + 1;


    rc = rte_srs_set_data (session1, "toto", iov, 3);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to set data on session1\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }

    printf ("(rank: %d)\texchanging data\n", my_rank);

    rc = rte_srs_exchange_data (session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to prepare data publishing\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }

    for (i = 0; i < rte_group_size(group_world); i++) {
        if (my_rank == i)
            continue;

        target_ec = rte_group_index_to_ec (group_world, i);

        rc = rte_srs_get_data (session1, target_ec, "toto",
                               &databuff, &buff_size);
        if (rc != RTE_SUCCESS) {
            fprintf (stderr, "Error: Modex wait failed\n");
            retval = EXIT_FAILURE;
            goto cleanup;
        }

        printf ("(rank: %d)\treceived srs data from rank %d\n\t\tbuffer size = %d\n", my_rank, i, buff_size);

        my_iov[0].iov_base  = &buf2;
        my_iov[0].count     = strlen(buf1) + 1;
        my_iov[0].type      = rte_datatype_int8_t;

        my_iov[1].iov_base  = &buf2;
        my_iov[1].count     = 4;
        my_iov[1].type      = rte_datatype_int8_t;

        my_iov[2].iov_base  = &buf2;
        my_iov[2].count     = strlen(buf4) + 1;
        my_iov[2].type      = rte_datatype_int8_t;

        offset = 0;

        if (0 < buff_size) {
            rte_unpack (&my_iov[0], databuff, &offset);
            if (NULL != my_iov[0].iov_base){
                printf ("(rank: %d)\tiov_base[0]: %s\n", my_rank, (char*)my_iov[0].iov_base);
            }
            else
                printf ("Warning: my_iov.iov_base is NULL!\n");

            rte_unpack (&my_iov[1], databuff, &offset);
            if (NULL != my_iov[1].iov_base){
                printf ("(rank: %d)\tiov_base[1]: %s\n", my_rank, (char*)my_iov[1].iov_base);
            }
            else
                printf ("Warning: my_iov[1].iov_base is NULL!\n");
            rte_unpack (&my_iov[2], databuff, &offset);
            if (NULL != my_iov[2].iov_base){
                printf ("(rank: %d)\tiov_base[2]: %s\n", my_rank, (char*)my_iov[2].iov_base);
            }
            else
                printf ("Warning: my_iov.iov_base is NULL!\n");
         }

        if (rc != RTE_SUCCESS) {
            fprintf (stderr, "Error: Impossible to subscribe for data\n");
            retval = EXIT_FAILURE;
            goto cleanup;
        }
    }

cleanup:
    fflush (stderr);
    fflush (stdout);
    rte_finalize ();

    return retval;
}

