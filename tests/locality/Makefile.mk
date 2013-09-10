## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
##                         All rights reserved.

bin_PROGRAMS += rte_locality_test rte_locality_to_str_test

rte_locality_test_SOURCES = tests/locality/rte_locality.c
rte_locality_test_LDADD = librte.la

rte_locality_to_str_test_SOURCES = tests/locality/rte_locality_to_str.c
rte_locality_to_str_test_LDADD = librte.la
