#ifndef BSW_DLFCN_HH
#define BSW_DLFCN_HH

#include <bsw/predef.h>
#if !defined(PREDEF_OS_WINDOWS)
#include <dlfcn.h>
#else
/**
 * @file dlfcn.h
 * @brief libdl api for win32.
 *
 * @author Ramiro Polla
 * @version 1.0.0
 * @date
 * @note
 * @since
 *
 * dlfcn-win32
 * Copyright (c) 2007 Ramiro Polla
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <bsw/export.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Relocations are performed when the object is loaded. */
#define RTLD_NOW    0


/* Relocations are performed at an implementation-defined time.
 * Windows API does not support lazy symbol resolving (when first reference
 * to a given symbol occurs). So RTLD_LAZY implementation is same as RTLD_NOW.
 */
#define RTLD_LAZY   RTLD_NOW


/* All symbols are available for relocation processing of other modules. */
#define RTLD_GLOBAL (1 << 1)


/* All symbols are not made available for relocation processing by other modules. */
#define RTLD_LOCAL  (1 << 2)


/* These two were added in The Open Group Base Specifications Issue 6.
 * Note: All other RTLD_* flags in any dlfcn.h are not standard compliant.
 */


/* The symbol lookup happens in the normal global scope. */
#define RTLD_DEFAULT    ((void *)0)

/* Specifies the next object after this one that defines name. */
#define RTLD_NEXT       ((void *)-1)


/**
 *  \brief Structure filled in by dladdr()
 *
 */
typedef struct dl_info
{
   const char *dli_fname;  /* Filename of defining object (thread unsafe and reused on every call to dladdr) */
   void       *dli_fbase;  /* Load address of that object */
   const char *dli_sname;  /* Name of nearest lower symbol */
   void       *dli_saddr;  /* Exact value of nearest symbol */
} Dl_info;


/**
 *  \brief Gain access to an executable object file
 *
 *  \details https://man7.org/linux/man-pages/man3/dlopen.3.html
 *
 */
BSW_EXPORT void *dlopen(const char *file, int mode);


/**
 *  \brief Close a symbol table handle.
 *
 *  \details https://man7.org/linux/man-pages/man3/dlopen.3.html
 */
BSW_EXPORT int dlclose(void *handle);


/**
 *  \brief Get the address of a symbol from a symbol table handle.
 *
 *  \details https://man7.org/linux/man-pages/man3/dlsym.3.html
 */
BSW_EXPORT void *dlsym(void *handle, const char *name);


/**
 *  \brief Get diagnostic information.
 *
 *  \details https://man7.org/linux/man-pages/man3/dlerror.3.html
 */
BSW_EXPORT char *dlerror(void);


/**
 *  \brief Translate address to symbolic information (no POSIX standard)
 *
 *  \details https://man7.org/linux/man-pages/man3/dladdr.3.html
 */
BSW_EXPORT int dladdr(const void *addr, Dl_info *info);

#ifdef __cplusplus
}
#endif


#endif
#endif
