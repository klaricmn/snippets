AC_PREREQ(2.63)

AC_INIT([libsst], [1.0.0], [matt@klaric.com])
AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_PATH_PROG(CXXTEST, [cxxtestgen.pl])
if test -z "$CXXTEST"; then AC_MSG_ERROR([cxxtest not found]); fi

AC_LANG([C++])

AC_CHECK_HEADER([boost/bimap.hpp], [], AC_MSG_ERROR([boost/bimap.hpp not found]))
AC_CHECK_HEADER([cxxtest/TestSuite.h], [], AC_MSG_ERROR([cxxtest/TestSuite.h not found]))

AM_CXXFLAGS="-Wall -Wextra -g -rdynamic"
AC_SUBST([AM_CXXFLAGS])

AC_CONFIG_FILES([
                 Makefile
                 doc/Makefile
                 src/Makefile
                 tests/Makefile
                 ])

AC_OUTPUT
