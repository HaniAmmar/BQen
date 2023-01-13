#include "php_bqen.h"

static const zend_function_entry bqen_functions[] = {PHP_FE(BQen_Render, arginfo_BQen_Render) PHP_FE_END};

zend_module_entry bqen_module_entry = {
    STANDARD_MODULE_HEADER, PHP_BQEN_EXTNAME,          bqen_functions, NULL, NULL, NULL, NULL, NULL,
    PHP_BQEN_VERSION,       STANDARD_MODULE_PROPERTIES};

#ifdef COMPILE_DL_BQEN
ZEND_GET_MODULE(bqen)
#endif

PHP_FUNCTION(BQen_Render) {
    using QTagBit = Qentem::Array<Qentem::Template::TagBit>;
    using QHArray = Qentem::HArray<QTagBit, char>;

    static QHArray                    cache;
    static Qentem::StringStream<char> stream;
    static QTagBit                    tags_cache;
    QTagBit                          *tags;

    char         *template_str      = nullptr;
    char         *template_name     = nullptr;
    zval         *value             = nullptr;
    Qentem::SizeT template_str_len  = 0;
    Qentem::SizeT template_name_len = 0;

    ZEND_PARSE_PARAMETERS_START(2, 3)
    Z_PARAM_STRING(template_str, template_str_len)
    Z_PARAM_ZVAL(value)
    Z_PARAM_OPTIONAL
    Z_PARAM_STRING(template_name, template_name_len)
    ZEND_PARSE_PARAMETERS_END();

    stream.Clear();

    if (template_name_len != 0) {
        tags = &(cache.GetOrAdd(template_name, template_name_len));
    } else {
        tags_cache.Clear();
        tags = &tags_cache;
    }

    if ((template_str_len != 0) && (template_str != nullptr)) {
        if (Z_TYPE_P(value) == IS_ARRAY) {
            Qentem::Template::Render(template_str, template_str_len, static_cast<const BQen::BQ_ZVAL &>(*value), stream,
                                     *tags);
        } else if (Z_TYPE_P(value) == IS_STRING) {
            const Qentem::Value<char> qval = Qentem::JSON::Parse(Z_STRVAL_P(value), Z_STRLEN_P(value));

            Qentem::Template::Render(template_str, template_str_len, qval, stream, *tags);
        }
    }

    if (stream.Length() != 0) {
        RETURN_STRINGL(stream.First(), stream.Length());
    } else {
        RETURN_STRINGL(" ", 1);
    }
}
