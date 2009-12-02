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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "textcat.h"
#include "php_textcat.h"

/* {{{ zend_class_entry */
zend_class_entry *textcat_ce_TextCategorizer;
zend_class_entry *textcat_ce_TextCategorizerException;
/* }}} */

/* {{{ ZEND_BEGIN_ARG_INFO */
static
ZEND_BEGIN_ARG_INFO_EX(arginfo_TextCategorizer__construct, 0, 0, 0)
	ZEND_ARG_INFO(0, configFile)
ZEND_END_ARG_INFO();

static
ZEND_BEGIN_ARG_INFO_EX(arginfo_TextCategorizer_classify, 0, 0, 0)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO();
/* }}} */

/* {{{ TextCategorizer methods */
static zend_function_entry TextCategorizer_methods[] = {
	PHP_ME(TextCategorizer, __construct, arginfo_TextCategorizer__construct, ZEND_ACC_PUBLIC)
    PHP_ME(TextCategorizer, classify, arginfo_TextCategorizer_classify, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
/* }}} */ 

/* {{{ textcat_module_entry
 */
zend_module_entry textcat_module_entry = {
	STANDARD_MODULE_HEADER,
	"textcat",
	NULL,
	PHP_MINIT(textcat),
	PHP_MSHUTDOWN(textcat),
	NULL,
	NULL,
	PHP_MINFO(textcat),
	PHP_TEXTCAT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TEXTCAT
ZEND_GET_MODULE(textcat)
#endif

/* {{{ TEXTCAT_FROM_OBJECT */
#define TEXTCAT_FROM_OBJECT(intern, object) \
    { \
        php_textcat_object *obj = (php_textcat_object*) zend_object_store_get_object(object TSRMLS_CC); \
        intern = obj->textcat; \
        if (!intern) { \
			zend_throw_exception(textcat_ce_TextCategorizerException, "Invalid or unitialized textcat  object"); \
            RETURN_FALSE; \
        } \
    }
/* }}} */

/* {{{ textcat_object_dtor() */
static void textcat_object_dtor(void *object, zend_object_handle handle TSRMLS_DC)
{
	php_textcat_object *intern;

	intern = (php_textcat_object *) object;

	zend_hash_destroy(intern->zo.properties);
	FREE_HASHTABLE(intern->zo.properties);

	if (intern->textcat) {
        textcat_Done(intern->textcat);
	}	

	efree(object);
}
/* }}} */

/* {{{ textcat_object_new() */
static zend_object_value textcat_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	zend_object_value retval;
	php_textcat_object *intern;
	zval *tmp;

	intern = emalloc(sizeof(php_textcat_object));
	memset(intern, 0, sizeof(php_textcat_object));
	intern->zo.ce = class_type;

	ALLOC_HASHTABLE(intern->zo.properties);
	zend_hash_init(intern->zo.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(intern->zo.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, textcat_object_dtor, NULL, NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();

	return retval;
	
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(textcat)
{
	zend_class_entry ce;
	zend_class_entry *ce_exception;

	/* Register TextCategorizerException class (inherits Exception) */
	INIT_CLASS_ENTRY(ce, "TextCategorizerException", NULL);
	ce_exception = zend_exception_get_default();
	textcat_ce_TextCategorizerException = zend_register_internal_class_ex(&ce, ce_exception, ce_exception->name TSRMLS_DC); 

	/* Register TextCategorizer class */
	INIT_CLASS_ENTRY(ce, "TextCategorizer", TextCategorizer_methods);
	textcat_ce_TextCategorizer = zend_register_internal_class(&ce TSRMLS_CC);
	textcat_ce_TextCategorizer->create_object = textcat_object_new;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(textcat)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(textcat)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "textcat support", "enabled");
	php_info_print_table_row(2, "textcat version", PHP_TEXTCAT_VERSION);
	php_info_print_table_end();

}
/* }}} */

/* {{{ proto void TextCategorizer::__construct(string $configFile) */
PHP_METHOD(TextCategorizer, __construct)
{
    char *configfile;
    int configfile_len;
	php_textcat_object *obj;
	zval *this;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &configfile, &configfile_len) == FAILURE) {
		RETURN_FALSE;
	}

	obj = (php_textcat_object *) zend_object_store_get_object(getThis() TSRMLS_DC);

	obj->textcat = textcat_Init(configfile); 

	if (!obj->textcat) zend_throw_exception(textcat_ce_TextCategorizerException, "could not initialize textcat resource");

}
/* }}} */


/* {{{ proto void TextCategorizer::classify(string $text) */
PHP_METHOD(TextCategorizer, classify)
{
    char *text, *result, *match;
    char **matches;
    int text_len, i;
    void *intern;
    size_t buffer_sz;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &text, &text_len) == FAILURE) {
        RETURN_FALSE;
    }

    TEXTCAT_FROM_OBJECT(intern, getThis());

    result = textcat_Classify(intern, text, text_len);

    if (!strcmp(result, "SHORT")) {
        zend_throw_exception(textcat_ce_TextCategorizerException, "Supplied text is shorter than 25 chars");
        RETURN_NULL();
    }

    RETURN_STRING(result, 1);

}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
