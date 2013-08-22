lib_LTLIBRARIES += librte.la

librte_la_SOURCES = \
    src/librte/rte.c

librte_la_LIBADD = ${librte_BACKENDS} ${librte_BACKEND_LIBS}
