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
    using QTagBit = Qentem::Array<Qentem::Template::TagBit<char>>;

    static Qentem::StringStream<char> template_ss;
    static QTagBit                    tags_cache;

    size_t template_str_len = 0;
    char * template_str     = nullptr;
    zval * value            = nullptr;

    ZEND_PARSE_PARAMETERS_START(2, 2)
    Z_PARAM_STRING(template_str, template_str_len)
    Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    template_ss.Clear();
    tags_cache.Clear();

    if ((template_str_len != 0) && (template_str != nullptr)) {
        if (Z_TYPE_P(value) == IS_ARRAY) {
            Qentem::Template::Render(template_str, template_str_len,
                                     static_cast<const BQen::BQ_ZVAL *>(value),
                                     &template_ss, &tags_cache);
        } else if (Z_TYPE_P(value) == IS_STRING) {
            const Qentem::Value<char> qval =
                Qentem::JSON::Parse(Z_STRVAL_P(value), Z_STRLEN_P(value));

            Qentem::Template::Render(template_str, template_str_len, &qval,
                                     &template_ss, &tags_cache);
        }
    }

    RETURN_STRINGL(template_ss.First(), template_ss.Length());
}
