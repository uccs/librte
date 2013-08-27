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
       AC_CHECK_LIB([open-rte], [orte_init])
       AS_IF([test "$ac_cv_lib_open_rte_orte_init" = yes],
             [rte_orte_cflags=" -I$orte_include_path"
              rte_orte_cflags+=" -I$orte_include_path/openmpi"
              rte_orte_cflags+=" -I$orte_include_path/openmpi/opal/mca/hwloc/hwloc152/hwloc/include"
              rte_orte_cflags+=" -I$orte_include_path/openmpi/opal/mca/event/libevent2021/libevent"
              AC_SUBST([LIBRTE_ORTE_CFLAGS],[$rte_orte_cflags])
              AC_SUBST([LIBRTE_ORTE_LIBADD],["-lopen-rte"])
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
             [rte_stci_cflags=" -I$stci_include_path"
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
