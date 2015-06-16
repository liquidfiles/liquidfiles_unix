#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(liquidfiles_unix, 0.1)
AM_INIT_AUTOMAKE(liquidfiles, 0.1)
AC_OUTPUT(Makefile src/Makefile src/base/Makefile src/io/Makefile src/lf/Makefile src/cmd/Makefile src/ui/Makefile)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
clean_CPPFLAGS=$CPPFLAGS
clean_LDFLAGS=$LDFLAGS
clean_LIBS=$LIBS
CURL_LIBS="-lcurl"
CURL_CONFIG_PATH="curl-config"
AC_ARG_WITH(curl,
AC_HELP_STRING([--with-curl=PATH],[search for curl in PATH]),
               [OPT_CURL="$withval"])

AC_ARG_ENABLE(static,
AC_HELP_STRING([--enable-static],[Link with curl static library]),
               [STATIC="1"])


dnl Add a nonempty path to the compiler flags
if test -n "$OPT_CURL"; then
   CPPFLAGS="$CPPFLAGS -I$OPT_CURL/include"
   LDFLAGS="$LDFLAGS -L$OPT_CURL/lib$libsuff"
   if test -e $OPT_CURL/curl-config; then
     CURL_CONFIG_PATH="$OPT_CURL/curl-config"
   else
     CURL_CONFIG_PATH="$OPT_CURL/bin/curl-config"
   fi
fi

if test "$STATIC" = "1" ; then
  CURL_LIBS=`$CURL_CONFIG_PATH --static-libs`
  if test -n "$OPT_CURL" ; then
    if test -e $OPT_CURL/lib/libssl.a ; then
      CURL_LIBS=${CURL_LIBS//-lssl/$OPT_CURL/lib/libssl.a}
    fi
    if test -e $OPT_CURL/lib/libcrypto.a ; then
      CURL_LIBS=${CURL_LIBS//-lcrypto/$OPT_CURL/lib/libcrypto.a}
    fi
  fi
  CURL_LIBS="$CURL_LIBS -lpthread -ldl"
fi

AC_CHECK_HEADER(curl/curl.h,
  [
  dnl curl/curl.h was found
  HAVE_CURL_H="1"
  ],
  [
    dnl curl/curl.h was not found, restore the flags
    CPPFLAGS=$clean_CPPFLAGS
    LDFLAGS=$clean_LDFLAGS]
  )

if test "$HAVE_CURL_H" == "1"
then
  dnl both header and lib were found!

  LIBS=$CURL_LIBS$clean_LIBS

  dnl replace 'HAVE_CURL' in the automake makefile.ams
  AMFIXLIB="1"
  AC_MSG_NOTICE([found curl.h header])
  curl_curl_msg="enabled"
else
  AC_MSG_WARN([configure not found curl header file])
  CPPFLAGS=$clean_CPPFLAGS
  LDFLAGS=$clean_LDFLAGS
  LIBS=$clean_LIBS
fi

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_OUTPUT
