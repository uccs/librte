## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
##                         All rights reserved.

AM_CPPFLAGS = $(LTDLINCL) -I$(top_ompi_builddir) -I$(top_ompi_builddir)/rte/include

bin_PROGRAMS += \
    rte_modex_test \
    rte_group_world_test \
    rte_p2p_test \
    rte_self_test \
    rte_barrier_test

rte_modex_test_SOURCES = tests/comm/modex/modex_test.c
rte_modex_test_LDADD = librte.la

rte_group_world_test_SOURCES = tests/comm/group/group_world.c
rte_group_world_test_LDADD = librte.la

rte_p2p_test_SOURCES = tests/comm/p2p/p2p.c
rte_p2p_test_LDADD = librte.la

rte_self_test_SOURCES = tests/comm/p2p/self.c
rte_self_test_LDADD = librte.la

##rte_persistent_request_test_SOURCES = tests/comm/p2p/persistent_request.c
##rte_persistent_request_test_LDADD = librte.la

rte_barrier_test_SOURCES = tests/comm/coll/barrier.c
rte_barrier_test_LDADD = librte.la
