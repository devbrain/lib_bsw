#if !defined(BSW_WARN_PUSH_)
#   error "'bsw/warn/push' include not detected or already closed with other 'pop'; "
#endif

#undef BSW_WARN_PUSH_



#if defined(PREDEF_COMPILER_GCC)
#pragma GCC diagnostic pop
#elif defined(PREDEF_COMPILER_VISUALC)
#pragma warning(pop)
#elif defined(PREDEF_COMPILER_CLANG)
#pragma clang diagnostic pop
#else
#endif


