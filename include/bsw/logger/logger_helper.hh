#ifndef BSW_LOGGER_HELPER_HH
#define BSW_LOGGER_HELPER_HH

#include <memory>

#include "bsw/logger/record.hh"
#include "bsw/logger/multi_setter.hh"

#include "bsw/export.h"

namespace bsw::logger
{
    void BSW_EXPORT disable();
    void BSW_EXPORT post_record(const std::shared_ptr<record>& record);

} // ns logger


#define EV_LOGGER_TRACE(SEVERITY, MODULE, FUNCTION, FILE, LINE, ...)	                                            \
  do {		                                                                            							\
    ::bsw::logger::locus locus_ (MODULE, FILE, FUNCTION, LINE);			                                            \
    std::shared_ptr<logger::record> rec = std::make_shared<logger::record> (SEVERITY, locus_);			            \
    ::bsw::logger::priv::multi_setter_c ms (*rec.get ());					                                        \
    ms.set (__VA_ARGS__) ;						                                                                	\
    ::bsw::logger::post_record (rec);							                                                            \
  } while (0)

#if !defined(TRACE_LEVEL)
#   if !(defined NDEBUG)
#      define TRACE_LEVEL 6
#   else
#      define TRACE_LEVEL 6
#   endif
#endif // TRACE_LEVEL



#if TRACE_LEVEL > 1
#define EV_LOGGER_TRACE_EVLOG_FATAL(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eFATAL, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_FATAL(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if TRACE_LEVEL > 2
#define EV_LOGGER_TRACE_EVLOG_ERROR(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eERROR, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_ERROR(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if TRACE_LEVEL > 3
#define EV_LOGGER_TRACE_EVLOG_WARNING(MODULE, FUNCTION, FILE, LINE, ...) \
  EV_LOGGER_TRACE (logger::eWARNING, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_WARNING(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if TRACE_LEVEL > 4
#define EV_LOGGER_TRACE_EVLOG_NOTICE(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eNOTICE, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_NOTICE(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if TRACE_LEVEL > 5
#define EV_LOGGER_TRACE_EVLOG_INFO(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eINFO, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_INFO(MODULE, FUNCTION, FILE, LINE, ...)                 ((void)0)
#endif

#if TRACE_LEVEL > 6
#define EV_LOGGER_TRACE_EVLOG_DEBUG(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eDEBUG, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_DEBUG(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#endif
