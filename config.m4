dnl config.m4 for extension dogma

dnl This was hacked together to a working state from the small amount of
dnl documentation available. Improvements welcome!

PHP_ARG_WITH(dogma, for dogma support,
[  --with-dogma             Include dogma support])

if test "$PHP_DOGMA" != "no"; then
	DOGMA_CFLAGS=`pkg-config --cflags libdogma`
	DOGMA_LDFLAGS=`pkg-config --libs libdogma`
	PHP_EVAL_LIBLINE([$DOGMA_LDFLAGS], DOGMA_SHARED_LIBADD)
	PHP_CHECK_LIBRARY(dogma, dogma_init, [], [ AC_MSG_ERROR([dogma library not found]) ], [$DOGMA_LDFLAGS])
	PHP_NEW_EXTENSION(dogma, dogma.c, $ext_shared, [], [$DOGMA_CFLAGS])
	PHP_SUBST(DOGMA_SHARED_LIBADD)
fi
