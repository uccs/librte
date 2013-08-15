# Copyright (c) 2009-2010 UT-Battelle, LLC. All rights reserved.

if HAVE_ORTE
librte_BACKENDS += librte-orte.la
endif
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
          src/backend/orte/rte_finilize.c \
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

librte_orte_la_CFLAGS=-I ${srcdir}/src/backend/orte -I ${srcdir}/include

librte_orte_la_LIBADD = LIBOPENRTE

librte_orte_la_DEPENDENCIES =

librte_orte_la_LDFLAGS = \
    $(OMPI_LIBMPI_EXTRA_LDFLAGS)
