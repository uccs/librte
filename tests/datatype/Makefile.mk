## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.

bin_PROGRAMS += rte_get_datatype_size_test

rte_get_datatype_size_test_SOURCES = tests/datatype/rte_get_datatype_size.c
rte_get_datatype_size_test_LDADD = librte.la
