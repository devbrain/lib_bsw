#ifndef BSW_LOGGER_HH
#define BSW_LOGGER_HH

#include "bsw/macros.hh"
#include "bsw/logger/logger_helper.hh"
#include "bsw/logger/function_name.hh"

/*! 
 * Allows emitting a message of a specific severity.
 * - \c SEVERITY is one of a list of six severity levels, as follows:
 *   - EVLOG_FATAL (most severe)
 *   - EVLOG_CRITICAL
 *   - EVLOG_ERROR
 *   - EVLOG_WARNING
 *   - EVLOG_NOTICE
 *   - EVLOG_INFO
 *   - EVLOG_DEBUG (least severe)
 * - args is a list containing at least 1 and a maximum of 32 arguments.
 *
 * If the logger cannot convert the passed arguments into strings, a compilation error is issued.
 */

#if defined(LOGGER_MODULE_NAME)
#define EVLOG_MODULE_NAME STRINGIZE(LOGGER_MODULE_NAME)
#else
#define EVLOG_MODULE_NAME ""
#endif

#define EVLOG_TRACE(SEVERITY, ...)  PPCAT (EV_LOGGER_TRACE_,SEVERITY)  (EVLOG_MODULE_NAME, FUNCTION_NAME, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
