## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
##                         All rights reserved.

lib_LTLIBRARIES += librte.la

librte_la_SOURCES = \
    src/librte/rte.c

include_HEADERS = \
    include/rte.h

librte_la_LIBADD = \
    ${librte_BACKENDS} \
    ${librte_BACKEND_LIBS} \
    librte_util.la
