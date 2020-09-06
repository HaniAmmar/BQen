#include "php_bqen.h"

static const zend_function_entry bqen_functions[] = {
    PHP_FE(BQen_Render, arginfo_BQen_Render) PHP_FE_END};

zend_module_entry bqen_module_entry = {STANDARD_MODULE_HEADER,
                                       PHP_BQEN_EXTNAME,
                                       bqen_functions,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       NULL,
                                       PHP_BQEN_VERSION,
                                       STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_BQEN
ZEND_GET_MODULE(bqen)
#endif

PHP_FUNCTION(BQen_Render) {
    char * tempale_str     = nullptr;
    size_t tempale_str_len = 0;

    zval *value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STRING(tempale_str, tempale_str_len)
    Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if ((tempale_str_len == 0) && (tempale_str == nullptr)) {
        RETURN_EMPTY_STRING();
    }

    if (Z_TYPE_P(value) == IS_ARRAY) {
        Qentem::String temp = Qentem::Template<BQen::BQ_ZVAL>::Render(
            tempale_str, static_cast<Qentem::ULong>(tempale_str_len),
            static_cast<const BQen::BQ_ZVAL *>(value));

        RETURN_STRINGL(temp.Char(), temp.Length());
    }

    if (Z_TYPE_P(value) == IS_STRING) {
        Qentem::Value qval(Qentem::JSON::Parse(
            Z_STRVAL_P(value), static_cast<Qentem::ULong>(Z_STRLEN_P(value))));

        Qentem::String temp = Qentem::Template<>::Render(
            tempale_str, static_cast<Qentem::ULong>(tempale_str_len), &qval);

        RETURN_STRINGL(temp.Char(), temp.Length());
    }

    RETURN_STRINGL(tempale_str, tempale_str_len);
}
