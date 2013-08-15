bin_PROGRAMS += rte_locality_test

rte_locality_test_SOURCES = tests/locality/rte_locality.c
rte_locality_test_LDADD = librte.la
