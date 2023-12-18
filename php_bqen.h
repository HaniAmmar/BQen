#ifndef _PHP_BQEN_H
#define _PHP_BQEN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define PHP_BQEN_EXTNAME "BQen"
#define PHP_BQEN_VERSION "2.2.4"

#define QENTEM_AVX2 1
#define QENTEM_SSE2 1

#include <php.h>

#define QENTEM_SIZE_T
namespace Qentem {
using SizeT = size_t;
}

#include "bqen.hpp"

extern zend_module_entry bqen_module_entry;
#define phpext_bqen_ptr &bqen_module_entry

#ifdef MAY_BE_STRING
#include "bqen_arginfo.h"
#else
#define arginfo_BQen_Render NULL
#endif

#ifndef RETURN_THROWS
#define RETURN_THROWS RETURN_NULL
#endif

PHP_FUNCTION(BQen_Render);

#endif
