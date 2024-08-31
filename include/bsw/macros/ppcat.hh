//
// Created by igor on 8/31/24.
//

#ifndef BSW_MACROS_PPCAT_HH
#define BSW_MACROS_PPCAT_HH


/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

#define ANONYMOUS_VAR(X) PPCAT(X, __LINE__)



#endif
