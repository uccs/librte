## Copyright (c) 2013      UT-Battelle, LLC. All rights reserved.

include src/backend/orte/Makefile.mk
include src/backend/stci/Makefile.mk
include src/backend/pmi/Makefile.mk

noinst_LTLIBRARIES += ${librte_BACKENDS}
