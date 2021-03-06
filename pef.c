#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pef.h"

static int le_pef;


zval **GET_TRACK_VARS(uint type, char * name){
    zval * arr;
    zval ** rval;
    zend_hash_find(HASH_OF(PG(http_globals)[type]), name, strlen(name)+1, (void **)&rval);
    //ZVAL_STRINGL(return_value, Z_STRVAL_PP(rval), Z_STRLEN_PP(rval), 1);
    //char *val=Z_STRVAL_PP(rval);
    return rval;
}


/**
 * 给该扩展定义一个名为confirm_pef_compiled的导出函数。
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION(confirm_pef_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "pef", arg);
	RETURN_STRINGL(strg, len, 0);
}

PHP_FUNCTION(run){
    char *appPath;
    zval c;
    
    //zend_get_executed_filename(TSRMLS_CC)
    
    //获取内核中及PHP中定义的常量
    if(zend_get_constant("APP_PATH",sizeof("APP_PATH")-1,&c TSRMLS_CC)){
       
        spprintf(&appPath,0,"%s\n%s\n","常量APP_PATH：",Z_STRVAL(c));
        //RETURN_STRING(appPath,0);
        
    }else{
        zend_error(E_WARNING,"APP_PATH没有定义！");
        RETURN_FALSE;
    }


    //获取超级全局变量的值
    
    RETURN_STRING(Z_STRVAL_PP(GET_TRACK_VARS(TRACK_VARS_SERVER,"PHP_SELF")),0);

}


PHP_MINIT_FUNCTION(pef)
{
    //如果没有这句，PHP中实例化config类时就会提示：class not found
    ZEND_MODULE_STARTUP_N(pef_config) (INIT_FUNC_ARGS_PASSTHRU);
    ZEND_MODULE_STARTUP_N(pef_db) (INIT_FUNC_ARGS_PASSTHRU);
    ZEND_MODULE_STARTUP_N(pef_model) (INIT_FUNC_ARGS_PASSTHRU);
    ZEND_MODULE_STARTUP_N(pef_controller) (INIT_FUNC_ARGS_PASSTHRU);
    ZEND_MODULE_STARTUP_N(pef_view) (INIT_FUNC_ARGS_PASSTHRU);
    /* startup components */
    //PEF_STARTUP(cfg);
    
    
    return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(pef)
{
	return SUCCESS;
}
PHP_RINIT_FUNCTION(pef)
{
	return SUCCESS;
}
PHP_RSHUTDOWN_FUNCTION(pef)
{
	return SUCCESS;
}


PHP_MINFO_FUNCTION(pef)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pef support", "enabled");
	php_info_print_table_end();

}

//扩展的函数列表，并将其引入到PHP内部。
const zend_function_entry pef_functions[] = {
	PHP_FE(confirm_pef_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(run,NULL)
        PHP_FE_END	/* Must be the last line in pef_functions[] */
};
zend_module_entry pef_module_entry = {
	STANDARD_MODULE_HEADER,
	"pef",
	pef_functions,
	PHP_MINIT(pef),
	PHP_MSHUTDOWN(pef),
	PHP_RINIT(pef),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pef),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pef),
	PHP_PEF_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PEF
ZEND_GET_MODULE(pef)
#endif

