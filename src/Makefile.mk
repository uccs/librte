# Copyright (c) 2009-2013 UT-Battelle, LLC. All rights reserved.
#                         All rights reserved.

lib_LTLIBRARIES =
librte_BACKENDS =

CFLAGS += -I ${srcdir}/include

include src/backend/Makefile.mk
include src/librte/Makefile.mk
