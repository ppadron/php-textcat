/*
 +----------------------------------------------------------------------+
 | PHP Version 5                                                        |
 +----------------------------------------------------------------------+
 | Copyright (c) 1997-2007 The PHP Group                                |
 +----------------------------------------------------------------------+
 | This source file is subject to version 3.01 of the PHP license,      |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.php.net/license/3_01.txt                                  |
 | If you did not receive a copy of the PHP license and are unable to   |
 | obtain it through the world-wide-web, please send a note to          |
 | license@php.net so we can mail you a copy immediately.               |
 +----------------------------------------------------------------------+
 | Author: Pedro Padron <ppadron@php.net>                               |
 +----------------------------------------------------------------------+
*/

#ifndef PHP_TEXTCAT_H
#define PHP_TEXTCAT_H


#define PHP_TEXTCAT_VERSION "0.1"

extern zend_module_entry textcat_module_entry;
#define phpext_textcat_ptr &textcat_module_entry

#define PHP_TEXTCAT_API

#ifdef ZTS
#include "TSRM.h"
#endif

typedef struct _php_textcat_object {
    zend_object zo;
    void *textcat;
} php_textcat_object;

extern zend_class_entry* textcat_ce_TextCategorizer;
extern zend_class_entry* textcat_ce_TextCategorizerException;

PHP_MINIT_FUNCTION(textcat);
PHP_MSHUTDOWN_FUNCTION(textcat);
PHP_RINIT_FUNCTION(textcat);
PHP_RSHUTDOWN_FUNCTION(textcat);
PHP_MINFO_FUNCTION(textcat);

PHP_METHOD(TextCategorizer, __construct);
PHP_METHOD(TextCategorizer, classify);

#ifdef ZTS
#define TEXTCAT_G(v) TSRMG(texcat_globals_id, zend_textcat_globals *, v)
#else
#define TEXTCAT_G(v) (textcat_globals.v)
#endif

#endif /* PHP_TEXTCAT_H */
