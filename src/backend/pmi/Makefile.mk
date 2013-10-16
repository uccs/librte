## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.

if HAVE_PMI
librte_BACKENDS += librte-pmi.la
librte_BACKEND_LIBS += ${LIBRTE_PMI_LIBADD}

librte_pmi_la_CFLAGS = \
    -I${srcdir}/src/backend/pmi \
    ${LIBRTE_PMI_CFLAGS}

librte_pmi_la_LIBADD = ${LIBRTE_PMI_LIBADD}
librte_pmi_la_LDFLAGS = ${LIBRTE_PMI_LDFLAGS}

librte_pmi_la_SOURCES = \
          src/backend/pmi/rte_abort.c \
          src/backend/pmi/rte_barrier.c \
          src/backend/pmi/rte_dt.c \
          src/backend/pmi/rte_cmp_ec.c \
          src/backend/pmi/rte_finalize.c \
          src/backend/pmi/rte_get_ec_hostname.c \
          src/backend/pmi/rte_get_ec_index.c \
          src/backend/pmi/rte_get_ec_locality.c \
          src/backend/pmi/rte_get_ec_node_name.c \
          src/backend/pmi/rte_get_my_ec.c \
          src/backend/pmi/rte_get_session_dir.c \
          src/backend/pmi/rte_group_index_to_ec.c \
          src/backend/pmi/rte_group_rank.c \
          src/backend/pmi/rte_group_size.c \
          src/backend/pmi/rte_init.c \
          src/backend/pmi/rte_modex.c \
          src/backend/pmi/rte_progress.c \
          src/backend/pmi/rte_recv.c \
          src/backend/pmi/rte_recv_nbcb.c \
          src/backend/pmi/rte_send.c \
          src/backend/pmi/rte_send_nbcb.c
endif
