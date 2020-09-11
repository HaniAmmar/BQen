#ifndef PHP_BQEN_H_
#define PHP_BQEN_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define PHP_BQEN_EXTNAME "BQen"
#define PHP_BQEN_VERSION "1.0.5"

#include "Template.hpp"
////////////////////
#include <php.h>
///////////////////
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
