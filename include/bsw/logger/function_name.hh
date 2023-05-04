#ifndef LOGGER_FUNCTION_NAME_HH
#define LOGGER_FUNCTION_NAME_HH

#if defined(__SUNPRO_C) || defined(__SUNPRO_CC) 
// works only with -features=extensions compiler flag
#define FUNCTION_NAME __func__
#define PRETTY_FUNCTION_NAME FUNCTION_NAME
#elif defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))
# define FUNCTION_NAME __FUNCTION__
# define PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(_MSC_VER) && (_MSC_VER >= 1020)
# define FUNCTION_NAME __FUNCTION__
# define PRETTY_FUNCTION_NAME __FUNCDNAME__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define FUNCTION_NAME __FUNCTION__
# define PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define FUNCTION_NAME __FUNCSIG__
# define PRETTY_FUNCTION_NAME FUNCTION_NAME
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define FUNCTION_NAME __FUNCTION__
# define PRETTY_FUNCTION_NAME FUNCTION_NAME
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define FUNCTION_NAME __FUNC__
# define PRETTY_FUNCTION_NAME FUNCTION_NAME
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define FUNCTION_NAME __func__
# define PRETTY_FUNCTION_NAME FUNCTION_NAME
#else
# define FUNCTION_NAME "(unknown function)"
# define PRETTY_FUNCTION_NAME FUNCTION_NAME
#endif

#endif
