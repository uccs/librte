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
    char buf1[]                    = "buf1";
    rte_iovec_t iov[1];
    rte_iovec_t my_iov;
    rte_group_t             group_world;
    rte_ec_handle_t         target_ec;
    void *databuff                  = NULL;
    int i, buff_size;
    uint32_t offset;
    char* value = NULL;

    rc = rte_init (&argc, &argv, &group_world);

    value = getenv("RTE_DEBUG");

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
    iov[0].count    = strlen (buf1) +1;

    rc = rte_srs_set_data (session1, "toto", iov, 1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to set data on session1\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }

    rc = rte_srs_exchange_data (session1);
    if (rc != RTE_SUCCESS) {
        fprintf (stderr, "Error: Impossible to prepare data publishing\n");
        retval = EXIT_FAILURE;
        goto cleanup;
    }

    for (i = 0; i < rte_group_size(group_world); i++) {

        target_ec = rte_group_index_to_ec (group_world, i);

        rc = rte_srs_get_data (session1, target_ec, "toto",
                               &databuff, &buff_size);
        if (rc != RTE_SUCCESS) {
            fprintf (stderr, "Error: Modex wait failed\n");
            retval = EXIT_FAILURE;
            goto cleanup;
        }

        printf ("(toto) Buffer size = %d\n", buff_size);

        my_iov.iov_base  = NULL;
        my_iov.count     = 0;
        my_iov.type      = rte_datatype_int8_t;

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

    fprintf (stdout, "Data received\n");

cleanup:
    fflush (stderr);
    fflush (stdout);
    rte_finalize ();

    return retval;
}

