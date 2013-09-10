## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.
##                         All rights reserved.

lib_LTLIBRARIES =
noinst_LTLIBRARIES =
librte_BACKENDS =
librte_BACKEND_LIBS =

CFLAGS += -I$(top_srcdir)/include

include src/backend/Makefile.mk
include src/librte/Makefile.mk
include src/util/Makefile.mk
