
#if defined(BSW_WARN_PUSH_)
# error "Not ending 'bsw/warn/push' include detected; every 'push' must ends with 'pop'; "
#else
# define BSW_WARN_PUSH_
#endif

#if !defined(WARN_PREDEF_COMPILER_GCC)
    #if defined(__GNUC__) && !defined(__clang__)
        # if defined(__GNUC_PATCHLEVEL__)
            #  define WARN_PREDEF_COMPILER_GCC (__GNUC__ * 10000 \
                            + __GNUC_MINOR__ * 100 \
                            + __GNUC_PATCHLEVEL__)
        # else
            #  define WARN_PREDEF_COMPILER_GCC (__GNUC__ * 10000 \
                            + __GNUC_MINOR__ * 100)
        # endif
    # endif
#endif

#if !defined(WARN_PREDEF_COMPILER_CLANG)
#if defined(__clang__)
#  define WARN_PREDEF_COMPILER_CLANG (__clang_major__ * 10000 \
                            + __clang_minor__ * 100 \
                            + __clang_patchlevel__)
#endif
#endif

#if !defined(WARN_PREDEF_COMPILER_VISUALC)
    #if defined(_MSC_VER)
        #if _MSC_VER == 1900
            #define WARN_PREDEF_COMPILER_VISUALC 2015
        #elif (_MSC_VER >= 1910) && (_MSC_VER < 2000)
            #define WARN_PREDEF_COMPILER_VISUALC 2017
        #else
            #error "Unknown compiler"
        #endif
    #endif
#endif


#if defined(WARN_PREDEF_COMPILER_GCC)
#pragma GCC diagnostic push
#elif defined(WARN_PREDEF_COMPILER_VISUALC)
#pragma warning(push)
#elif defined(WARN_PREDEF_COMPILER_CLANG)
#pragma clang diagnostic push
#else
#endif


