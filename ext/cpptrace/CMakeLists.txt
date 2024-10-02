include(FetchContent)
FetchContent_Declare(
        cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG        v0.6.1 # <HASH or TAG>
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(cpptrace)




