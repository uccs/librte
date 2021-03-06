#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([librte], [0.2], [])
AC_CONFIG_SRCDIR([include/rte.h])
AC_CONFIG_AUX_DIR([autoconf])
AC_CONFIG_MACRO_DIR([autoconf/m4])
AC_CONFIG_HEADERS([include/rte_config.h])
AM_INIT_AUTOMAKE([1.11 foreign no-define subdir-objects])
AM_SILENT_RULES([yes])
LT_INIT([])
AC_LANG([C])
# Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_GREP

AC_DEFUN([RTE_CHECK_OPAL_LIBEVENT_VERSION],[
    libevent_include_dir=$(grep MCA_event_IMPLEMENTATION_HEADER $2/opal_config.h | awk '{print $ 3}' | tr -d \"| xargs dirname)
    echo $libevent_include_dir
    AC_SUBST([$1_libevent_include_dir],[$libevent_include_dir])
])

AC_DEFUN([RTE_CHECK_OPAL_HWLOC_VERSION],[
    hwloc_include_dir=$(grep MCA_hwloc_IMPLEMENTATION_HEADER $2/opal_config.h | awk '{print $ 3}' | tr -d \"| xargs dirname)
    echo $hwloc_include_dir
    AC_SUBST([$1_hwloc_include_dir],[$hwloc_include_dir])
])

# Check for features.
# AC_ARG_ENABLE (feature, help-string, [action-if-given], [action-if-not-given])
AC_ARG_WITH([orte],
    [AS_HELP_STRING([--with-orte],
                   [enable the orte backend (default is no)])],
    [use_orte=$withval],
    [use_orte=no])

AC_ARG_WITH([orte-lib],
    [AS_HELP_STRING([--with-orte-lib],
                   [specify the path to the open-rte library])],
    [use_orte_lib=$withval],
    [use_orte_lib=no])

AC_ARG_WITH([orte-include],
    [AS_HELP_STRING([--with-orte-include],
                   [specify the path to the open-rte include headers])],
    [use_orte_include=$withval],
    [use_orte_include=no])

AC_ARG_WITH([stci],
    [AS_HELP_STRING([--with-stci],
                   [enable the stci backend (default is no)])],
    [use_stci=$withval],
    [use_stci=no])

AC_ARG_WITH([stci-lib],
    [AS_HELP_STRING([--with-stci-lib],
                   [specify the path to the stci library])],
    [use_stci_lib=$withval],
    [use_stci_lib=no])

AC_ARG_WITH([stci-include],
    [AS_HELP_STRING([--with-stci-include],
                   [specify the path to the stci include headers])],
    [use_stci_include=$withval],
    [use_stci_include=no])

AC_ARG_WITH([tests],
    [AS_HELP_STRING([--with-tests],
                   [build the rte tests (default is no)])],
    [with_tests=$withval],
    [with_tests=no])
AM_CONDITIONAL([WITH_RTE_TESTS], [test x$with_tests = xyes])

AC_ARG_WITH([pmi],
    [AS_HELP_STRING([--with-pmi],
                   [enable the pmi backend (default is no)])],
    [use_pmi=$withval],
    [use_pmi=no])

AC_ARG_WITH([pmi-lib],
    [AS_HELP_STRING([--with-pmi-lib],
                   [specify the path to the pmi library])],
    [use_pmi_lib=$withval],
    [use_pmi_lib=no])

AC_ARG_WITH([pmi-include],
    [AS_HELP_STRING([--with-pmi-include],
                   [specify the path to the pmi include headers])],
    [use_pmi_include=$withval],
    [use_pmi_include=no])

# Checks for libraries.

#save $LDFLAGS
LDFLAGS_SAVE=$LDFLAGS
#Save $LIBS
SAVE_LIBS=$LIBS
AS_IF([test "x$use_orte" != "xno"],
      [#dnl Set the default path to /usr
       orte_path="/usr"
       orte_lib_path="/usr/lib"
       orte_include_path="/usr/include"
       AS_IF([test "x$use_orte" != "xyes"],
             [orte_path=$use_orte
              orte_lib_path="$use_orte/lib"
              orte_include_path="$use_orte/include"])
       AS_IF([test "x$use_orte_lib" != "xno"],
             [orte_lib_path="$use_orte_lib"])
       AS_IF([test "x$use_orte_include" != "xno"],
             [orte_include_path="$use_orte_include"
             ],[])
       #dnl set LDFLAGS for the check
       LDFLAGS="-L$orte_lib_path"
       LIBS="-lopen-pal"
       AC_CHECK_LIB([open-rte], [orte_init])
       AS_IF([test "$ac_cv_lib_open_rte_orte_init" = yes],
             [RTE_CHECK_OPAL_HWLOC_VERSION([rte_orte],[$orte_include_path/openmpi])
              RTE_CHECK_OPAL_LIBEVENT_VERSION([rte_orte],[$orte_include_path/openmpi])
              rte_orte_cflags=" -I$orte_include_path"
              rte_orte_cflags+=" -I$orte_include_path/openmpi"
              rte_orte_cflags+=" -I$orte_include_path/openmpi/$rte_orte_hwloc_include_dir/hwloc/include"
              rte_orte_cflags+=" -I$orte_include_path/openmpi/$rte_orte_libevent_include_dir/libevent"
              rte_orte_cflags+=" -I$orte_include_path/openmpi/$rte_orte_libevent_include_dir/libevent/include"
              AC_SUBST([LIBRTE_ORTE_CFLAGS],[$rte_orte_cflags])
              AC_SUBST([LIBRTE_ORTE_LIBADD],["-lopen-rte -lopen-pal"])
              AC_SUBST([LIBRTE_ORTE_LDFLAGS], ["-L$orte_lib_path"])
              AC_DEFINE([HAVE_ORTE], [1], [Define if we have ORTE])
             ],[
              AC_MSG_FAILURE([--with-orte was given, but test for orte failed])
             ])
      ])
AM_CONDITIONAL([HAVE_ORTE], [test "x$use_orte" != xno])

#reset LDFLAGS
LDFLAGS=$LDFLAGS_SAVE
LIBS=$SAVE_LIBS

AS_IF([test "x$use_stci" != "xno"],
      [#dnl set the default path to /usr
       stci_path="/usr"
       stci_lib_path="/usr/lib"
       stci_include_path="/usr/include"
       AS_IF([test "x$use_stci" != "xyes"],
             [stci_path=$use_stci
              stci_lib_path="$use_stci/lib"
              stci_include_path="$use_stci/include"])
       AS_IF([test "x$use_stci_lib" != "xno"],
             [stci_lib_path=$use_stci_lib])
       AS_IF([test "x$use_stci_include" != "xno"],
             [stci_include_path=$use_stci_include])
       #dnl set LDFLAGS for the check
       LDFLAGS="-L$stci_lib_path"
       AC_CHECK_LIB([stci], [stci_init])
       AS_IF([test "$ac_cv_lib_stci_stci_init" = yes],
             [RTE_CHECK_OPAL_HWLOC_VERSION([rte_stci],[$stci_include_path/openmpi])
              RTE_CHECK_OPAL_LIBEVENT_VERSION([rte_stci],[$stci_include_path/openmpi])
              rte_stci_cflags=" -I$stci_include_path"
              rte_stci_cflags+=" -I$stci_include_path/openmpi"
              rte_stci_cflags+=" -I$stci_include_path/openmpi/opal/mca/hwloc/hwloc152/hwloc/include"
              rte_stci_cflags+=" -I$stci_include_path/openmpi/opal/mca/event/libevent2021/libevent"
              AC_SUBST([LIBRTE_STCI_CFLAGS],[$rte_stci_cflags])
              AC_SUBST([LIBRTE_STCI_LIBADD], ["-lstci"])
              AC_SUBST([LIBRTE_STCI_LDFLAGS],["-L$stci_lib_path"])
              AC_DEFINE([HAVE_STCI], [1], [Define if we have stci])
             ],[
              AC_MSG_FAILURE([--with-stci was given, but test for stci failed])
             ])
       ])
AM_CONDITIONAL([HAVE_STCI], [test "x$use_stci" != xno])

#reset LDFLAGS
LDFLAGS=$LDFLAGS_SAVE
#reset $LIBS to save value
LIBS=$SAVE_LIBS

# Check for pmi libraries and flavors
AS_IF([test "x$use_pmi" != "xno"],
## this is hardcoded for titan in the moment
##       AC_SUBST([LIBRTE_PMI_CFLAGS],[$CRAY_PMI_INCLUDE_OPTS])
##       AC_SUBST([LIBRTE_PMI_LDFLAGS],[$CRAY_PMI_POST_LINK_OPTS])
##       AC_SUBST([LIBRTE_PMI_LIBADD], ["-lpmi"])
      [#dnl set the default path to /usr
       pmi_path="/usr"
       pmi_lib_path="/usr/lib"
       pmi_include_path="/usr/include"
       AS_IF([test "x$use_pmi" != "xyes"],
             [pmi_path=$use_pmi
              pmi_lib_path="$pmi_lib_path"
              pmi_include_path="$pmi_include_path"])
       AS_IF([test "x$use_pmi_lib" != "xno"],
             [pmi_lib_path=$use_pmi_lib])
       AS_IF([test "x$use_pmi_include" != "xno"],
             [pmi_include_path=$use_pmi_include])
       #dnl set LDFLAGS for the check
       LDFLAGS="-L$pmi_lib_path"
       AC_CHECK_LIB([pmi], [PMI_Init])
       AS_IF([test "$ac_cv_lib_pmi_PMI_Init" = yes],
             [rte_pmi_cflags=" -I$pmi_include_path"
              AC_SUBST([LIBRTE_PMI_CFLAGS],[$rte_pmi_cflags])
              AC_SUBST([LIBRTE_PMI_LIBADD], ["-lpmi"])
              AC_SUBST([LIBRTE_PMI_LDFLAGS],["-L$pmi_lib_path"])
              AC_DEFINE([HAVE_PMI], [1], [Define if we have PM])
             ],[
              AC_MSG_FAILURE([--with-pmi was given, but test for PMI failed])
             ])
       #dnl check if we are running on a cray
       AC_CHECK_LIB([pmi], [PMI_CRAY_Get_rank_in_app])
       AS_IF([test "$ac_cv_lib_pmi_PMI_CRAY_Get_rank_in_app" = yes],
             [#dnl we have the cray extensions so it should be save to assume
              #dnl that we are running on a cray box
              rte_cray_pmi=1
              rte_slurm_pmi=0
             ],[#dnl if we are not running on a cray it must be a slurm box
              rte_cray_pmi=0
              rte_slurm_pmi=1
             ])
       AC_DEFINE_UNQUOTED([HAVE_CRAY_PMI], [$rte_cray_pmi],
                          [Build RTE for the cray])
       AC_DEFINE_UNQUOTED([HAVE_SLURM_PMI], [$rte_slurm_pmi],
                          [Build rte for slurm])
       ])
AM_CONDITIONAL([HAVE_PMI], [test "x$use_pmi" != xno])

#reset LDFLAGS
LDFLAGS=$LDFLAGS_SAVE

#reset $LIBS to save value
LIBS=$SAVE_LIBS

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
