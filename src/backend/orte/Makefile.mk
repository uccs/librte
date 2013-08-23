# Copyright (c)      2013 UT-Battelle, LLC. All rights reserved.

if HAVE_ORTE
librte_BACKENDS += librte-orte.la
librte_BACKEND_LIBS += ${LIBRTE_ORTE_LIBADD}

librte_orte_la_CFLAGS = \
    -I${srcdir}/src/backend/orte \
    ${LIBRTE_ORTE_CFLAGS}

librte_orte_la_LIBADD = ${LIBRTE_ORTE_LIBADD}
librte_orte_la_LDFLAGS = ${LIBRTE_ORTE_LDFLAGS}

librte_orte_la_SOURCES = \
          src/backend/orte/rte_init.c \
          src/backend/orte/rte_group_rank.c \
          src/backend/orte/rte_group_size.c \
          src/backend/orte/rte_get_my_ec.c \
          src/backend/orte/rte_get_ec_locality.c \
          src/backend/orte/rte_cmp_ec.c \
          src/backend/orte/rte_get_ec_index.c \
          src/backend/orte/rte_get_ec_node_name.c \
          src/backend/orte/rte_get_ec_hostname.c \
          src/backend/orte/rte_get_session_dir.c \
          src/backend/orte/rte_abort.c \
          src/backend/orte/rte_progress.c \
          src/backend/orte/rte_modex.c \
          src/backend/orte/rte_finalize.c \
          src/backend/orte/rte_dt.h \
          src/backend/orte/rte_dt.c \
          src/backend/orte/rte_internal.h \
          src/backend/orte/rte_internal.c \
          src/backend/orte/p2p/rte_send.c \
          src/backend/orte/p2p/rte_send_nbcb.c \
          src/backend/orte/p2p/rte_recv.c \
          src/backend/orte/p2p/rte_recv_nbcb.c \
          src/backend/orte/coll/rte_barrier.c \
          src/backend/orte/rte_group_index_to_ec.c
endif
