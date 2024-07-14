#ifndef BSW_LOGGER_HELPER_HH
#define BSW_LOGGER_HELPER_HH

#include <memory>

#include "bsw/logger/record.hh"
#include "bsw/logger/multi_setter.hh"

#include "bsw/export.h"

namespace bsw::logger {
	void BSW_EXPORT disable ();
	void BSW_EXPORT post_record (const std::shared_ptr<record>& record);

} // ns logger


#define EV_LOGGER_TRACE(SEVERITY, MODULE, FUNCTION, FILE, LINE, ...)                        \
  do {                                                                                      \
    using namespace ::bsw::logger;                                                          \
    locus locus_ (MODULE, FILE, FUNCTION, LINE);                                            \
    auto rec = std::make_shared<record> (SEVERITY, locus_);                                 \
    priv::multi_setter_c ms (*rec.get ());                                                  \
    ms.print (__VA_ARGS__) ;                                                                \
    post_record (rec);                                                                      \
  } while (0)

#if !defined(EVLOG_TRACE_LEVEL)
#   if !(defined NDEBUG)
#      define EVLOG_TRACE_LEVEL 7
#   else
#      define EVLOG_TRACE_LEVEL 6
#   endif
#endif // EVLOG_TRACE_LEVEL

#if EVLOG_TRACE_LEVEL > 1
#define EV_LOGGER_TRACE_EVLOG_FATAL(MODULE, FUNCTION, FILE, LINE, ...)    \
  EV_LOGGER_TRACE (::bsw::logger::eFATAL, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_FATAL(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if EVLOG_TRACE_LEVEL > 2
#define EV_LOGGER_TRACE_EVLOG_ERROR(MODULE, FUNCTION, FILE, LINE, ...)    \
  EV_LOGGER_TRACE (::bsw::logger::eERROR, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_ERROR(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if EVLOG_TRACE_LEVEL > 3
#define EV_LOGGER_TRACE_EVLOG_WARNING(MODULE, FUNCTION, FILE, LINE, ...) \
  EV_LOGGER_TRACE (::bsw::logger::eWARNING, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_WARNING(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if EVLOG_TRACE_LEVEL > 4
#define EV_LOGGER_TRACE_EVLOG_NOTICE(MODULE, FUNCTION, FILE, LINE, ...)    \
  EV_LOGGER_TRACE (::bsw::logger::eNOTICE, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_NOTICE(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#if EVLOG_TRACE_LEVEL > 5
#define EV_LOGGER_TRACE_EVLOG_INFO(MODULE, FUNCTION, FILE, LINE, ...)    \
  EV_LOGGER_TRACE (::bsw::logger::eINFO, MODULE, FUNCTION, FILE, LINE,  ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_INFO(MODULE, FUNCTION, FILE, LINE, ...)                 ((void)0)
#endif

#if EVLOG_TRACE_LEVEL > 6
#define EV_LOGGER_TRACE_EVLOG_DEBUG(MODULE, FUNCTION, FILE, LINE, ...)	\
  EV_LOGGER_TRACE (logger::eDEBUG, MODULE, FUNCTION, FILE, LINE, ##__VA_ARGS__)
#else
#define EV_LOGGER_TRACE_EVLOG_DEBUG(MODULE, FUNCTION, FILE, LINE, ...)            ((void)0)
#endif

#endif
