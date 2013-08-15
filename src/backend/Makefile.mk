# Copyright (c) 2009-2013 UT-Battelle, LLC. All rights reserved.
#                         All rights reserved.

include src/backend/orte/Makefile.mk
include src/backend/stci/Makefile.mk

lib_LTLIBRARIES += ${librte_BACKENDS}
