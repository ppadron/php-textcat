PHP_ARG_WITH(textcat,for TEXTCAT support,
[  --with-textcat[=DIR]       Include TEXTCAT support])

if test "$PHP_TEXTCAT" != "no"; then

  if test "$PHP_TEXTCAT" != "yes"; then
    TEXTCAT_SEARCH_DIRS=$PHP_TEXTCAT
  else
    TEXTCAT_SEARCH_DIRS="/usr/local /usr"
  fi

  for i in $TEXTCAT_SEARCH_DIRS; do
    if test -f $i/include/textcat/textcat.h; then
      TEXTCAT_DIR=$i
      TEXTCAT_INCDIR=$i/include/textcat
    elif test -f $i/include/textcat.h; then
      TEXTCAT_DIR=$i
      TEXTCAT_INCDIR=$i/include
    fi
  done

  if test -z "$TEXTCAT_DIR"; then
    AC_MSG_ERROR(Cannot find libtextcat)
  fi

  TEXTCAT_LIBDIR=$TEXTCAT_DIR/$PHP_LIBDIR

  PHP_ADD_LIBRARY_WITH_PATH(textcat, $TEXTCAT_LIBDIR, TEXTCAT_SHARED_LIBADD)
  PHP_ADD_INCLUDE($TEXTCAT_INCDIR)


  PHP_NEW_EXTENSION(textcat, textcat.c, $ext_shared)
  PHP_SUBST(TEXTCAT_SHARED_LIBADD)
  AC_DEFINE(HAVE_TEXTCAT,1,[ ])
fi
