## Copyright (c) 2009-2013 UT-Battelle, LLC. All rights reserved.

include tests/comm/Makefile.mk
include tests/locality/Makefile.mk
include tests/datatype/Makefile.mk

bin_PROGRAMS += \
    rte_hostname_test

rte_hostname_test_SOURCES = tests/rte_hostname.c
rte_hostname_test_LDADD = librte.la
