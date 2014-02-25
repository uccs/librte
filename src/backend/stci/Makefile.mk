## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.

if HAVE_STCI
librte_BACKENDS += librte-stci.la
librte_BACKEND_LIBS += ${LIBRTE_STCI_LIBADD}

librte_stci_la_CFLAGS = \
    -I${srcdir}/src/backend/stci \
    ${LIBRTE_STCI_CFLAGS}

librte_stci_la_LIBADD = ${LIBRTE_STCI_LIBADD}
librte_stci_la_LDFLAGS = ${LIBRTE_STCI_LDFLAGS}

librte_stci_la_SOURCES = \
          src/backend/stci/coll/rte_barrier.c \
          src/backend/stci/p2p/rte_recv.c \
          src/backend/stci/p2p/rte_recv_nb.c \
          src/backend/stci/p2p/rte_send.c \
          src/backend/stci/p2p/rte_send_nb.c \
          src/backend/stci/return_code_to_string.c \
          src/backend/stci/rte_cmp_ec.c \
          src/backend/stci/rte_dt.c \
          src/backend/stci/rte_ec_to_str.c \
          src/backend/stci/rte_abort.c \
          src/backend/stci/rte_fini.c \
          src/backend/stci/rte_get_job_id.c \
          src/backend/stci/rte_get_datatype_size.c \
          src/backend/stci/rte_get_ec_hostname.c \
          src/backend/stci/rte_get_ec_index.c \
          src/backend/stci/rte_get_ec_locality.c \
          src/backend/stci/rte_get_ec_node_id.c \
          src/backend/stci/rte_get_ec_node_name.c \
          src/backend/stci/rte_get_my_ec.c \
          src/backend/stci/rte_get_session_dir.c \
          src/backend/stci/rte_group_index_to_ec.c \
          src/backend/stci/rte_group_rank.c \
          src/backend/stci/rte_group_size.c \
          src/backend/stci/rte_init.c \
          src/backend/stci/rte_modex.c \
          src/backend/stci/rte_request.c \
          src/backend/stci/rte_show_help.c
endif
