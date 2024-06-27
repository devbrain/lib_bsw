set(BUILD_TESTING OFF CACHE INTERNAL "")
fetchcontent_declare(
        PFR
        GIT_REPOSITORY "https://github.com/boostorg/pfr.git"
        GIT_TAG 2.2.0
        UPDATE_COMMAND ""
)
fetchcontent_makeavailable(PFR)

