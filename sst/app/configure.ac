dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([sst], [1.0.0], [matt@klaric.com])
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([foreign])


AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_PROG_LIBTOOL

CXXFLAGS="-Wall"

## switch to the c++ compiler otherwise the header check will barf
AC_LANG_CPLUSPLUS

## AC_CHECK_HEADER([sst/SortedSumTree.h], [], AC_MSG_ERROR([Header sst/SortedSumTree.h not found]))
## AC_CHECK_LIB([sst], [main], [], AC_MSG_ERROR([Library libsst not found.]))

##### begin check for libsst
AC_ARG_WITH(sst,
  [[  --with-sst[=DIR]     support for search libsst [in DIR]]])

have_sst="no"
if test -n "$with_sst"
then
  CPPFLAGS="$CPPFLAGS -I$with_sst/include/sst"
  LDFLAGS="$LDFLAGS -L$with_sst/lib"
fi

AC_CHECK_HEADER(SortedSumTree.h, have_sst="maybe")
if test "$have_sst" != "no"
then
  AC_CHECK_LIB(sst, main,[LIBS="$LIBS -lsst"
    AC_DEFINE(HAVE_sst, 1, [Define if you have libsst])
    have_sst="yes"
  ],[have_sst="no"])
else
   AC_MSG_ERROR([Could not find SortedSumTree.h using $CPPFLAGS])
fi

if test "$have_sst" != "yes"
then
    AC_MSG_ERROR([Could not find libsst using $LDFLAGS])
fi
##### end check for libsst

AC_HEADER_STDBOOL
AC_C_CONST

AC_CONFIG_FILES([Makefile
				src/Makefile
				])
AC_OUTPUT
